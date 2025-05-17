
 /*
    An ECS manages a list of entities, each with a set of components
    components are stored contigiously so looping over a given type of component is effecient
    
    useful when many disjointed systems need to act on a similar large set of organized data
    usually game engines

    7/24/24 notes
    - component IDs are static -- every instance of ECS shares them. this could be fixed
    - max comp and entity numbers are also static -- ideally template these
    - added tags that don't alloc anything but get a component id, moderately tested

    4/29/25 notes
    - ECS is header only now and max entities is templated, max components still should be
    - Improving the archetype feature; archetypes now hold references to each component in the archetype
        - this is not well tested yet
 */

#ifndef ECS_h
#define ECS_h
#include <bitset>
#include <vector>
#include <tuple>
#include <cassert>
#include <stdint.h>

const uint32_t MAX_COMPONENTS = 64;

typedef uint64_t entID;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

template <size_t MAX_ENTITIES = 0x40000>
class ECS {
public:
    entID newEntity() {
        if (!freelist.empty()){
            uint32_t index = (uint32_t)freelist.back();
            freelist.pop_back();
            entities[index].id = replace_entity_id_at(index);
            return entities[index].id;
        }
        entities.push_back({gen_entity_id_at((uint32_t)entities.size()), ComponentMask()});
        return entities.back().id;
    }
    void removeEntity(entID& i)  {
        uint32_t index = get_entity_index(i);
        assert(index < entities.size());
        assert(i == entities.at(index).id);
        entities.at(index).mask.reset();
        clear_entity_id(entities.at(index).id);
        i = entities.at(index).id;
        freelist.push_back(index);
    }
    bool entityValid(entID i) const {
        if (get_entity_index(i) == 0xFFFFFFFF){return false;}
        return i == (entities.at(get_entity_index(i)).id);
    }
    
    size_t numEntities() { 
        return entities.size() - freelist.size();
    }    
    size_t numComponents() { 
        return p_componentCounter;
    }
    
    template <class T>
    void addTag(entID i) {
        assert(entityValid(i));
        int compID = get_comp_id<T>();
        while (compID >= (int)pools.size()){
            pools.emplace_back(ObjectPool(true));
        }
        if (pools[compID].valid()){
            pools[compID] = ObjectPool(false);
        }
        entities.at(get_entity_index(i)).mask.set(compID);
    }
    template <class T, typename... ArgTypes>
    T& addComp(entID i, ArgTypes... args) {
        assert(entityValid(i));
        int compID = get_comp_id<T>();
        while (compID >= (int)pools.size()){
            pools.emplace_back(ObjectPool(true));
        }
        if (pools[compID].nobuf()){
            pools[compID] = ObjectPool(sizeof(T));
        }
        T* component = new (pools[compID].get(get_entity_index(i))) T(args...);
        entities.at(get_entity_index(i)).mask.set(compID);
        return *component;
    }
    template <class T>
    T& getComp(entID i) {
        assert(entityValid(i));
        int compID = get_comp_id<T>();
        assert(entities.at(get_entity_index(i)).mask.test(compID));
        return *((T*) pools.at(compID).get(get_entity_index(i)));
    }
    template <class T>
    T* tryGetComp(entID i) {
        assert(entityValid(i));
        int compID = get_comp_id<T>();
        if (!entities.at(get_entity_index(i)).mask.test(compID)){
            return nullptr;
        }
        return ((T*) pools.at(compID).get(get_entity_index(i)));
    }
    template <class T>
    T& addOrGetComp(entID i) {
        assert(entityValid(i));
        int compID = get_comp_id<T>();
        if (!entities.at(get_entity_index(i)).mask.test(compID)){
            return addComp<T>(i);
        }
        return *((T*) pools.at(compID).get(get_entity_index(i)));
    }
    template <class T>
    void removeComp(entID i) {
        assert(entityValid(i));
        entities.at(get_entity_index(i)).mask.reset(get_comp_id<T>());
    }

    template <typename ... Comps>
    struct Archetype {
        friend class ECS;
        template<typename T>
        T& get() {
            return *std::get<T*>(components);
        }
        template<typename T>
        const T& get() const {
            return *std::get<T*>(components);
        }
    private:
        std::tuple<Comps*...> components;
    };

    // --- Populate an Archetype with pointers ---
    template<typename... Types>
    void fill_archetype(entID id, Archetype<Types...>* arch) {
        assert(entityValid(id));
        assert(arch != nullptr);
        arch->components = std::make_tuple(tryGetComp<Types>(id)...);
    }

    // ----------- SceneView -----------
    template <class... Types>
    class SceneView {
        friend class ECS;
        ECS & home;
        ComponentMask mask;
        bool filter;
        SceneView(ECS* h) : home(*h) {
            /* TODO error checking: make sure comps exist here first */
            filter = !(sizeof...(Types) == 0);
            mask.reset();
            if (filter){
                int ids[] = { 0, home.get_comp_id<Types>() ... };
                for (size_t i = 0; i < sizeof...(Types); i++){
                    mask.set(ids[i+1]);
                }
            }
        }
    public:
        class ViewIterator {
        private:
            ECS & home;
            uint32_t index;
            ComponentMask mask;
            bool filter;
            bool maskValid(ComponentMask& other){
                return !(((other & mask) ^ mask).any());
            }
        public:
            ViewIterator(ECS & h, uint32_t i, ComponentMask m, bool f) : home(h), index(i), mask(m), filter(f) {
                if (index == 0xFFFFFFFF){return;}
                if (home.entities.size() == 0) {
                    index = -1;
                    return;
                }
                auto next = home.entities.at(index);
                if (maskValid(next.mask)) {return;}
                ++(*this);
            }
            entID operator*() const {
                if (index == 0xFFFFFFFF){ return 0xFFFFFFFFFFFFFFFF; }
                return home.entities.at(index).id;
            }
            bool operator==(ViewIterator const& other) const {
                return this->index == other.index;
            }
            bool operator!=(ViewIterator const& other) const {
                return this->index != other.index;
            }
            ViewIterator& operator++(){
                if (index == 0xFFFFFFFF){return *this;}
                while (1) {
                    index++;
                    if (index >= (uint32_t)home.entities.size()){
                        index = 0xFFFFFFFF;
                        return *this;
                    }
                    auto next = home.entities.at(index);
                    if (home.entityValid(next.id) && (!filter || (maskValid(next.mask)))){
                        return *this;
                    }
                }
            }
        };
        
        const ViewIterator begin() const {
            return ViewIterator(home, 0, mask, filter);
        }
        const ViewIterator end() const {
            return ViewIterator(home, -1, mask, filter);
        }
    };

    template <class... Types>
    SceneView<Types...> view() {
        return SceneView<Types...>(this);
    }

    // ----------- ArchetypeSceneView -----------
    template <class... Types>
    class ArchetypeSceneView {
        friend class ECS;
        ECS& home;
        Archetype<Types...>* arch;
        ComponentMask mask;
        bool filter;
        ArchetypeSceneView(ECS* h, Archetype<Types...>* a) : home(*h), arch(a) {
            filter = !(sizeof...(Types) == 0);
            mask.reset();
            if (filter){
                int ids[] = { 0, home.get_comp_id<Types>()... };
                for (size_t i = 0; i < sizeof...(Types); i++){
                    mask.set(ids[i+1]);
                }
            }
        }
    public:
        class ViewIterator {
            ECS& home;
            Archetype<Types...>* arch;
            uint32_t index;
            ComponentMask mask;
            bool filter;
            bool maskValid(ComponentMask& other){
                return !(((other & mask) ^ mask).any());
            }
        public:
            ViewIterator(ECS& h, uint32_t i, ComponentMask m, bool f, Archetype<Types...>* a)
                : home(h), arch(a), index(i), mask(m), filter(f) {
                if (index == 0xFFFFFFFF){return;}
                if (home.entities.size() == 0) {
                    index = -1;
                    return;
                }
                auto next = home.entities.at(index);
                if (maskValid(next.mask)) { home.fill_archetype<Types...>(next.id, arch); return; }
                ++(*this);
            }
            entID operator*() const {
                if (index == 0xFFFFFFFF){ return 0xFFFFFFFFFFFFFFFF; }
                return home.entities.at(index).id;
            }
            bool operator==(ViewIterator const& other) const { return this->index == other.index; }
            bool operator!=(ViewIterator const& other) const { return this->index != other.index; }
            ViewIterator& operator++(){
                if (index == 0xFFFFFFFF){return *this;}
                while (1) {
                    index++;
                    if (index >= (uint32_t)home.entities.size()){
                        index = 0xFFFFFFFF;
                        return *this;
                    }
                    auto next = home.entities.at(index);
                    if (home.entityValid(next.id) && (!filter || (maskValid(next.mask)))){
                        home.fill_archetype<Types...>(next.id, arch);
                        return *this;
                    }
                }
            }
        };

        const ViewIterator begin() const { return ViewIterator(home, 0, mask, filter, arch); }
        const ViewIterator end() const { return ViewIterator(home, -1, mask, filter, arch); }
    };

    template <class... Types>
    ArchetypeSceneView<Types...> view(Archetype<Types...>& arch) {
        return ArchetypeSceneView<Types...>(this, &arch);
    }


private:
    // entity id parsing
    entID gen_entity_id_at(uint32_t index)  {
        return (entID{0} | index) << 32;
    }
    entID replace_entity_id_at(uint32_t index)  {
        assert(index < entities.size());
        entID res = entities.at(index).id;
        return ((res & ~0xFFFFFFFF00000000) | ((uint64_t)index << 32)) | ((res & 0xFFFFFFFF) + 1);
    }
    void clear_entity_id(entID& i) {
        auto index = get_entity_index(i);
        assert(index < entities.size());
        assert(i == entities.at(index).id);
        entID res = entities.at(index).id;
        i = ((res & 0xFFFFFFFF) + 1) | (0xFFFFFFFF00000000);
        entities[index].id = i;
    }
    static uint32_t get_entity_index(entID const& i) {
        return i >> 32;
    }
    // component ID
    unsigned int p_componentCounter{0};
    template <class T>
    int get_comp_id() {
        static int cid = p_componentCounter++;
        return cid;
    }
    // entity/mask list
    struct EntityData {
        entID id;
        ComponentMask mask;
    };
    std::vector<EntityData> entities;
    std::vector<size_t> freelist;

    // pools of components
    struct ObjectPool {

        static const size_t NEEDBUF = ((size_t)(-1));
        static const size_t INVALID = ((size_t)(-2));
        size_t obj_size;
        char* data{0};
        ObjectPool(bool tag) {
            obj_size = tag ? NEEDBUF : INVALID;
            data = 0;
        }
        ObjectPool(size_t s) {
            obj_size = s;
            data = new char[obj_size * MAX_ENTITIES];
        }
        ObjectPool(ObjectPool& other) = delete;
        ObjectPool(ObjectPool&& other) {
            this->obj_size = other.obj_size;
            this->data = other.data;
            other.obj_size = -1;
            other.data = 0;
        }
        ObjectPool(ObjectPool const& other) = delete;
        ObjectPool& operator=(ObjectPool&& other) {
            this->obj_size = other.obj_size;
            this->data = other.data;
            other.obj_size = -1;
            other.data = 0;
            return *this;
        }
        ~ObjectPool() {
            if (data) delete [] data;
        }
        bool valid() const {return obj_size == INVALID;}
        bool nobuf() const {return obj_size == NEEDBUF;}
        void* get(size_t i) {
            assert(i < MAX_ENTITIES);
            return data + (i * obj_size);
        }
    };
    
    std::vector<ObjectPool> pools;

};

#endif /* ECS_H */

//
//  ShaderContainer.h
//  graphics-library-interface
//
//  Created by Frank Collebrusco on 3/17/23.
//
//

#ifndef ShaderContainer_h
#define ShaderContainer_h
#include "Shader.h"
#include <unordered_map>

typedef uint32_t MAP_ID;

template <typename T>
class ID_Map {
private:
    MAP_ID counter;
    std::unordered_map<MAP_ID, T> map;
public:
    ID_Map(){
        counter = 0;
    }
    template <typename... Params>
    MAP_ID emplace(Params... args){
        MAP_ID res = counter++;
        map.emplace(std::make_pair(res, T(args...)));
        return res;
    }
//    MAP_ID emplace(T copy){
//        MAP_ID res = counter++;
//        map[res] = copy;
//        return res;
//    }
    T& get(MAP_ID id){
        return map[id];
    }
    void remove(MAP_ID id){
        map[id].destroy();
        map.erase(id);
    }
    T& operator[](MAP_ID id){
        return map[id];
    }
};


#endif /* ShaderContainer_h */

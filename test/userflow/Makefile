# this is barely necessary unless i add more tests but keeping it for now

.PHONY: test clean
test:
	./run.sh $(BRANCH) 2>&1 | tee run.log

clean:
	rm -f run.log

# flgl user-flow testbench — convenience wrapper around run.sh.
#   make test BRANCH=rework3    run the full user-flow test, teeing to run.log
#   make clean                  remove run.log
# (run.sh clones flgl from GitHub, so push the branch first.)

.PHONY: test clean
test:
	./run.sh $(BRANCH) 2>&1 | tee run.log

clean:
	rm -f run.log

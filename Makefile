init:
	wget http://plg.uwaterloo.ca/~usystem/pub/uSystem/u++-7.0.0.sh
	sudo sh u++-7.0.0.sh
	rm u++-7.0.0.sh
	@echo "u++ installed"

fmt:
	clang-format -i --verbose **/*.cc
	clang-format -i --verbose **/*.h

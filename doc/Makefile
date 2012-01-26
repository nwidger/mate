all: mc.dox mas.dox mvm.dox

mc.dox: mc.doxygen
	doxygen mc.doxygen

mas.dox: mas.doxygen
	doxygen mas.doxygen

mvm.dox: mvm.doxygen
	doxygen mvm.doxygen

clean:
	rm -rf mc/html mas/html mvm/html

.PHONY: mc.dox mas.dox mvm.dox

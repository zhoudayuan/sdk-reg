.PHONY: clean All

All:
	@echo "----------Building project:[ wujian100_open-driver - BuildSet ]----------"
	@
	@make -f "wujian100_open-driver.mk" MakeIntermediateDirs && make -f "wujian100_open-driver.mk" -j 4
	@echo Executing Post Build commands ...
	@export CDKPath="D:/C-Sky/CDK" ProjectPath="D:/wujian/fpga-bin/prj_hello/sdk/projects/tests/driver/CDK" && "D:/wujian/fpga-bin/prj_hello/sdk/projects/tests/driver/CDK//../../../../utilities//aft_build.sh"
	@echo Done

clean:
	@echo "----------Cleaning project:[ wujian100_open-driver - BuildSet ]----------"
	@make -f  "wujian100_open-driver.mk" clean

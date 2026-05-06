################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../code/Control.c" \
"../code/Filter.c" \
"../code/Flash.c" \
"../code/IMU.c" \
"../code/Init.c" \
"../code/Motor.c" \
"../code/My_Math.c" \
"../code/PID.c" \
"../code/VOFA.c" \
"../code/image.c" \
"../code/ips_show.c" \
"../code/ljy_key.c" \
"../code/ljy_menu.c" \
"../code/swj.c" 

COMPILED_SRCS += \
"code/Control.src" \
"code/Filter.src" \
"code/Flash.src" \
"code/IMU.src" \
"code/Init.src" \
"code/Motor.src" \
"code/My_Math.src" \
"code/PID.src" \
"code/VOFA.src" \
"code/image.src" \
"code/ips_show.src" \
"code/ljy_key.src" \
"code/ljy_menu.src" \
"code/swj.src" 

C_DEPS += \
"./code/Control.d" \
"./code/Filter.d" \
"./code/Flash.d" \
"./code/IMU.d" \
"./code/Init.d" \
"./code/Motor.d" \
"./code/My_Math.d" \
"./code/PID.d" \
"./code/VOFA.d" \
"./code/image.d" \
"./code/ips_show.d" \
"./code/ljy_key.d" \
"./code/ljy_menu.d" \
"./code/swj.d" 

OBJS += \
"code/Control.o" \
"code/Filter.o" \
"code/Flash.o" \
"code/IMU.o" \
"code/Init.o" \
"code/Motor.o" \
"code/My_Math.o" \
"code/PID.o" \
"code/VOFA.o" \
"code/image.o" \
"code/ips_show.o" \
"code/ljy_key.o" \
"code/ljy_menu.o" \
"code/swj.o" 


# Each subdirectory must supply rules for building sources it contributes
"code/Control.src":"../code/Control.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Control.o":"code/Control.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/Filter.src":"../code/Filter.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Filter.o":"code/Filter.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/Flash.src":"../code/Flash.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Flash.o":"code/Flash.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/IMU.src":"../code/IMU.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/IMU.o":"code/IMU.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/Init.src":"../code/Init.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Init.o":"code/Init.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/Motor.src":"../code/Motor.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/Motor.o":"code/Motor.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/My_Math.src":"../code/My_Math.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/My_Math.o":"code/My_Math.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/PID.src":"../code/PID.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/PID.o":"code/PID.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/VOFA.src":"../code/VOFA.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/VOFA.o":"code/VOFA.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/image.src":"../code/image.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/image.o":"code/image.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/ips_show.src":"../code/ips_show.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/ips_show.o":"code/ips_show.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/ljy_key.src":"../code/ljy_key.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/ljy_key.o":"code/ljy_key.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/ljy_menu.src":"../code/ljy_menu.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/ljy_menu.o":"code/ljy_menu.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"
"code/swj.src":"../code/swj.c" "code/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc26xb "-fD:/desktop/smart_car/ADS_Work_place/TC264_common_new_4_25/Debug/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -Y0 -N0 -Z0 -o "$@" "$<"
"code/swj.o":"code/swj.src" "code/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-code

clean-code:
	-$(RM) ./code/Control.d ./code/Control.o ./code/Control.src ./code/Filter.d ./code/Filter.o ./code/Filter.src ./code/Flash.d ./code/Flash.o ./code/Flash.src ./code/IMU.d ./code/IMU.o ./code/IMU.src ./code/Init.d ./code/Init.o ./code/Init.src ./code/Motor.d ./code/Motor.o ./code/Motor.src ./code/My_Math.d ./code/My_Math.o ./code/My_Math.src ./code/PID.d ./code/PID.o ./code/PID.src ./code/VOFA.d ./code/VOFA.o ./code/VOFA.src ./code/image.d ./code/image.o ./code/image.src ./code/ips_show.d ./code/ips_show.o ./code/ips_show.src ./code/ljy_key.d ./code/ljy_key.o ./code/ljy_key.src ./code/ljy_menu.d ./code/ljy_menu.o ./code/ljy_menu.src ./code/swj.d ./code/swj.o ./code/swj.src

.PHONY: clean-code


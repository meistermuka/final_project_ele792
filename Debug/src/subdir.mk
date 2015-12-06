################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/helpers.c \
../src/photoapp.c 

OBJS += \
./src/helpers.o \
./src/photoapp.o 

C_DEPS += \
./src/helpers.d \
./src/photoapp.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/user/local/include/gphoto2/gphoto2 -I"/home/muka/REPOS/GIT/final_project_ele792/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



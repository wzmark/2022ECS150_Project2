# Target library
lib := libuthread.a

objs:= uthread.o queue.o sem.o
objs+= context.o preempt.o 

# Define compilation toolchain
CC	= gcc
# General options
CFLAGS	:= -Wall -Wextra -MMD -Werror

# Variable for possible debugging, set D=1 to see messages
ifneq ($(D),1)
Q = @
endif

all: $(lib)
#Dep tracking
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

$(lib): $(objs)
	@echo "CC $@"
	$(Q)ar rcs -o $@ $^


%.o: %.c
	@echo "CC $@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<


clean:
	@echo "clean"
	$(Q)rm -f $(lib) $(objs) $(deps)

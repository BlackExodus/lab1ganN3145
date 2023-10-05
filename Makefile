.PHONY: all clean

APP=lab1ganN3145
CFLAGS=-Wall -Wextra -Werror -g

all: $(APP)

$(APP):
	gcc -o $(APP) $(CFLAGS) $(APP).c

clean:
	rm $(APP)


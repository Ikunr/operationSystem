# 自定义变量
# 变量赋值
OBJ=$(patsubst %.c, %.o, $(wildcard ./*.c))
TARGET=test_main

LDFALGS=-L./src_so -L./src_a
LIBS=-lSoMyAdd -lMyDiv

# 使用$(TARGET) 必须要加 '$' 符号
$(TARGET):$(OBJ)
	$(CC) $^ -o $@

# 模式匹配
%.o:%.c
	$(CC) -c $^ -o $@

# 伪文件 / 伪目标
.PHONY:	clean

# 清除编译出来的依赖文件 和 二进制文件
clean:
	@$(RM) *.o $(TARGET)

show:
	@echo $(RM)
	@echo $(CC)
	@echo $(CXX)
	@echo $(wildcard ./*.c)
	@echo $(patsubst %.c, %.o, $(wildcard ./*.c))
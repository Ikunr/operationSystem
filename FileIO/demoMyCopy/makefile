# 自定义变量
# 变量赋值
OBJS=$(patsubst %.c, %.o, $(wildcard ./*.c))
TARGET=mycopy

LDFALGS=-L./src_so -L./src_a
LIBS=-lSoMyAdd -lMyDiv

# 目录
PATHS:=/tmp/29main/
BIN:=/usr/local/bin/


# 使用$(TARGET) 必须要加 '$' 符号
$(TARGET):$(OBJS)
	$(CC) $^ -o $@

# 模式匹配
%.o:%.c
	$(CC) -c $^ -o $@

# 伪文件 / 伪目标
.PHONY:	clean

# install的功能作用:
# 1.创建目录，将可执行文件拷贝到指定目录(安装目录)
# 2.加全局可执行的路径
# 3.加全局的启停脚本
install:$(TARGET)
	@if [ -d $(PATHS) ];						\
		then echo $(PATHS) exist;			\
	else									\
		mkdir $(PATHS);						\
		cp $(TARGET) $(PATHS);				\
		ln -sv $(PATHS)$(TARGET) $(BIN);	\
	fi

# 清除编译出来的依赖文件 和 二进制文件
clean:
	@$(RM) $(OBJS) $(TARGET)
	@$(RM) -rf $(PATHS)
	@$(RM) $(BIN)$(TARGET)

show:
	@echo $(RM)
	@echo $(CC)
	@echo $(CXX)
	@echo $(wildcard ./*.c)
	@echo $(patsubst %.c, %.o, $(wildcard ./*.c))
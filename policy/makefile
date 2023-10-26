CC = gcc
CFLAGS = -Wall -Wextra -g

# 源碼目錄
SRCDIR = ./data_structure
# 頭檔目錄
INCDIR = $(SRCDIR)/int $(SRCDIR)/char $(SRCDIR)/access_tree
# 目標檔目錄
OBJDIR = obj

# 取得所有的源碼檔案，包括子目錄中的檔案
SRCFILES := $(wildcard $(SRCDIR)/**/*.c) createPolicy.c
# 將源碼檔案的路徑轉換為目標檔案的路徑
OBJFILES := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCFILES))
# 設定頭檔目錄的編譯選項
INCDIRS := $(addprefix -I, $(INCDIR))

TARGET = a.out

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(INCDIRS) $^ -o $@

$(OBJDIR)/%.o: %.c
	# 創建目標檔案所在的目錄
	@mkdir -p $(@D)
	# 使用編譯器編譯源碼檔案並輸出到目標檔案
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

clean:
	# 清理目標檔案和執行檔
	rm -rf $(OBJDIR) $(TARGET)

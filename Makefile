# 変数の定義
CC = gcc
CFLAGS = -Wall -g
SRCDIR = src
DISTDIR = dist

# ソースファイルとターゲットの定義
SRC = $(wildcard $(SRCDIR)/*.c)
TARGETS = $(patsubst $(SRCDIR)/%.c,$(DISTDIR)/%,$(SRC))

# ターゲットの定義
all: $(TARGETS)

# 実行ファイルの生成ルール
$(DISTDIR)/%: $(SRCDIR)/%.c
	@mkdir -p $(DISTDIR)
	$(CC) $(CFLAGS) $< -o $@

# クリーンアップのルール
clean:
	rm -rf $(DISTDIR)

.PHONY: all clean
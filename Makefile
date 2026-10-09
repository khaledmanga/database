format:
	clang-format -i $(wildcard \
		storage/*.cc \
		storage/*.h \
		bplustree/*.cc \
		bplustree/*.h \
		operators/*.cc \
		operators/*.h \
		common/*.h \
		query/lexer.cc \
		query/lexer.h \
		query/token.h \
		util/common.cc \
		util/common.h)

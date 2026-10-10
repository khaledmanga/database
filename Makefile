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
		query/schema.h \
		query/schema.cc \
		query/catalog.cc \
		query/catalog.h \
		query/binder.h \
		query/binder.cc \
		query/token.h \
		query/parser.h \
		query/parser.cc \
		util/common.cc \
		util/common.h)

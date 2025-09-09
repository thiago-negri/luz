CFLAGS := -I../include -L../lib -lluz -fsanitize=address -g

arena: arena.c
	$(CC) $< $(CFLAGS) -o $@

string_builder: string_builder.c
	$(CC) $< $(CFLAGS) -o $@

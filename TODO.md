- 2025-09-12 Add profiling, see puffin

- 2025-09-12 Change to a single translation unit to allow more optimization opportunities like inlining, probably a
             single header file with the implemetation embedded via a compilation flag like stb libraries

- 2025-09-12 :read !rg 'tnegri'
include/luz/allocator.h:	/* TODO(tnegri) 2025-09-09 Add a list of allocated objects that can be checked at any point to detect leaks. */
include/luz/allocator.h:/* TODO(tnegri) 2025-09-09 Docs and maybe return the context? (using libc to allocate anyway) */
include/luz/string.h:/* TODO(tnegri) 2025-09-09 Need easier ways to print strings to FILE*, update examples/string_list.c */

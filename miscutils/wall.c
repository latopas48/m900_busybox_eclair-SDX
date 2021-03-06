/* vi: set sw=4 ts=4: */
/*
 * wall - write a message to all logged-in users
 * Copyright (c) 2009 Bernhard Reutner-Fischer
 *
 * Licensed under GPLv2 or later, see file LICENSE in this tarball for details.
 */

#include "libbb.h"
#include <utmp.h>

int wall_main(int argc, char **argv) MAIN_EXTERNALLY_VISIBLE;
int wall_main(int argc UNUSED_PARAM, char **argv)
{
	struct utmp *ut;
	char *msg;
	int fd = argv[1] ? xopen(argv[1], O_RDONLY) : STDIN_FILENO;

	msg = xmalloc_read(fd, NULL);
	if (ENABLE_FEATURE_CLEAN_UP && argv[1])
		close(fd);
	setutent();
	while ((ut = getutent()) != NULL) {
		char *line;
		if (ut->ut_type != USER_PROCESS)
			continue;
		line = concat_path_file("/dev", ut->ut_line);
		xopen_xwrite_close(line, msg);
		free(line);
	}
	if (ENABLE_FEATURE_CLEAN_UP) {
		endutent();
		free(msg);
	}
	return EXIT_SUCCESS;
}

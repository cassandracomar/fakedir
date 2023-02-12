#include "common.h"

/**
 * @file        trivial_replacements.c
 * @author      Karim Vergnes <me@thesola.io>
 * @copyright   GPLv2
 * @brief       Set of trivial rewrites for system calls using paths.
 *
 * The following replacements are numerous and pretty much follow the same
 * pattern, so they have been grouped here for easier reading.
 */

#define RS_PARENT(p) resolve_symlink_parent(p, -1)

static char *rs_at_flagged(int fd, char *path, int flags)
{
    if (flags & AT_FDCWD && flags & AT_SYMLINK_NOFOLLOW)
        return resolve_symlink_parent(path, -1);
    else if (flags & AT_FDCWD)
        return resolve_symlink(path);
    else if (flags & AT_SYMLINK_NOFOLLOW)
        return resolve_symlink_parent(path, fd);
    else
        return resolve_symlink_at(fd, path);
}


int my_openat(int fd, char *name, int flags, int mode)
{
    DEBUG("openat(%s) was called.", name);
    return openat(fd, rewrite_path(name), flags, mode);
}

int my_lstat(char *path, struct stat *buf)
{
    DEBUG("lstat(%s) was called.", path);
    return lstat(RS_PARENT(path), buf);
}

int my_stat(char *path, struct stat *buf)
{
    DEBUG("stat(%s) was called.", path);
    return stat(resolve_symlink(path), buf);
}

int my_fstatat(int fd, char *path, struct stat *buf, int flag)
{
    DEBUG("fstatat(%s) was called.", path);
    return fstatat(fd, rewrite_path(path), buf, flag);
}

int my_access(char *path, int mode)
{
    DEBUG("access(%s) was called.", path);
    return access(rewrite_path(path), mode);
}

int my_faccessat(int fd, char *path, int mode, int flag)
{
    DEBUG("faccessat(%s) was called.", path);
    return faccessat(fd, rewrite_path(path), mode, flag);
}

int my_chflags(char *path, int flags)
{
    DEBUG("chflags(%s) was called.", path);
    return chflags(rewrite_path(path), flags);
}

int my_mkfifo(char *path, mode_t mode)
{
    DEBUG("mkfifo(%s) was called.", path);
    return mkfifo(rewrite_path(path), mode);
}

int my_chmod(char *path, mode_t mode)
{
    DEBUG("chmod(%s) was called.", path);
    return chmod(resolve_symlink(path), mode);
}

int my_fchmodat(int fd, char *path, mode_t mode, int flag)
{
    DEBUG("fchmodat(%s) was called.", path);
    return fchmodat(fd, rs_at_flagged(fd, path, flag), mode, flag);
}

int my_chown(char *path, uid_t owner, gid_t group)
{
    DEBUG("chown(%s) was called.", path);
    return chown(resolve_symlink(path), owner, group);
}

int my_lchown(char *path, uid_t owner, gid_t group)
{
    DEBUG("lchown(%s) was called.", path);
    return lchown(RS_PARENT(path), owner, group);
}

int my_fchownat(int fd, char *path, uid_t owner, gid_t group, int flag)
{
    DEBUG("fchownat(%s) was called.", path);
    return fchownat(fd, rs_at_flagged(fd, path, flag), owner, group, flag);
}

int my_link(char *path1, char *path2)
{
    DEBUG("link(%s, %s) was called.", path1, path2);
    return link(rewrite_path(path1), rewrite_path(path2));
}

int my_linkat(int fd1, char *path1, int fd2, char *path2, int flag)
{
    DEBUG("linkat(%s, %s) was called.", path1, path2);
    return linkat(fd1, rewrite_path(path1), fd2, rewrite_path(path2), flag);
}

int my_unlink(char *path)
{
    DEBUG("unlink(%s) was called.", path);
    return unlink(rewrite_path(path));
}

int my_unlinkat(int fd, char *path, int flag)
{
    DEBUG("unlinkat(%s) was called.", path);
    return unlinkat(fd, rewrite_path(path), flag);
}

int my_symlink(char *what, char *path)
{
    DEBUG("symlink(%s) was called.", path);
    return symlink(what, rewrite_path(path));
}

int my_symlinkat(char *what, int fd, char *path)
{
    DEBUG("symlinkat(%s) was called.", path);
    return symlinkat(what, fd, rewrite_path(path));
}

ssize_t my_readlink(char *path, char *buf, size_t bsz)
{
    DEBUG("readlink(%s) was called.", path);
    return readlink(RS_PARENT(path), buf, bsz);
}

ssize_t my_readlinkat(int fd, char *path, char *buf, size_t bsz)
{
    DEBUG("readlinkat(%s) was called.", path);
    return readlinkat(fd, rewrite_path(path), buf, bsz);
}

int my_open(char *name, int flags, int mode)
{
    DEBUG("open(%s) was called.", name);
    if (flags ^ (O_SYMLINK|O_NOFOLLOW))
        return open(RS_PARENT(name), flags, mode);
    else
        return open(resolve_symlink(name), flags, mode);
}

DIR *my_opendir(char *path)
{
    DEBUG("opendir(%s) was called.", path);
    return opendir(resolve_symlink(path));
}

// vim: ft=c.doxygen

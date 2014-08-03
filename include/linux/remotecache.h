#ifndef _LINUX_REMOTECACHE_H
#define _LINUX_REMOTECACHE_H

#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/mm.h>

struct remotecache_plug {
	struct list_head list;
};

struct remotecache_ops {
	int (*readpage) (struct file *, struct page *);
	int (*readpages) (struct file *, struct address_space *,
			struct list_head *pages, unsigned nr_pages);
	void (*releasepage) (struct page *page);
	void (*suspend) (void);
	void (*resume) (void);
};

#ifdef CONFIG_REMOTECACHE
extern struct remotecache_ops *remotecache_ops;

extern int remotecache_readpage(struct file *file, struct page *page);

extern int remotecache_readpages(struct file *, struct address_space *,
		struct list_head *pages, unsigned nr_pages);

extern void remotecache_register_ops(struct remotecache_ops *);

static inline void remotecache_releasepage(struct page *page)
{
	if (remotecache_ops && remotecache_ops->releasepage)
		remotecache_ops->releasepage(page);
}

static inline void remotecache_suspend(void)
{
	if (remotecache_ops && remotecache_ops->suspend)
		remotecache_ops->suspend();
}

static inline void remotecache_resume(void)
{
	if (remotecache_ops && remotecache_ops->resume)
		remotecache_ops->resume();
}
#else

static inline int remotecache_readpage(struct file *file, struct page *page)
{
	return page->mapping->a_ops->readpage(file, page);
}

static inline int remotecache_readpages(struct file *file, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	return mapping->a_ops->readpages(file, mapping, pages, nr_pages);
}

static inline void remotecache_register_ops(struct remotecache_ops *ops) { }

static inline void remotecache_releasepage(struct page *page) {}
static inline void remotecache_suspend(void) {}
static inline void remotecache_resume(void) {}
#endif

#endif /* _LINUX_REMOTECACHE_H */

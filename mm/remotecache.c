/*
 *	mm/remotecache.c
 *
 *	Remotecache support functions
 *
 *	Copyright (C) 2012
 *	Maxime Lorrillere <maxime.lorrillere@lip6.fr>
 *	LIP6 - Laboratoire d'Informatique de Paris 6
 */

#include <linux/export.h>
#include <linux/remotecache.h>

struct remotecache_ops *remotecache_ops = NULL;
EXPORT_SYMBOL(remotecache_ops);

int remotecache_readpage(struct file *file, struct page *page)
{
	if (remotecache_ops && remotecache_ops->readpage)
		return remotecache_ops->readpage(file, page);
	return page->mapping->a_ops->readpage(file, page);
}
EXPORT_SYMBOL(remotecache_readpage);

int remotecache_readpages(struct file *file, struct address_space *mapping,
		struct list_head *pages, unsigned nr_pages)
{
	if (remotecache_ops && remotecache_ops->readpages)
		return remotecache_ops->readpages(file, mapping, pages,
				nr_pages);
	return mapping->a_ops->readpages(file, mapping, pages, nr_pages);
}
EXPORT_SYMBOL(remotecache_readpages);

void remotecache_register_ops(struct remotecache_ops *ops)
{
	remotecache_ops = ops;
}
EXPORT_SYMBOL(remotecache_register_ops);

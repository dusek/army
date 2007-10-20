#ifndef __COLLECTION_MEMORY_H__
#define __COLLECTION_MEMORY_H__

class CollectionMemory: public Memory {
public:
	void add_memrange(addr_t start, addr_t end, Memory *engine);

private:
	Memory *get_engine(addr_t addr);
	std::vector<std::pair<std::pair<addr_t,addr_t>, Memory *> > engines;
}

#endif


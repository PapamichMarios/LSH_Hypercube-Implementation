#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <iostream>

#include "hash_node.h"
#include "fi.h"

template <typename K, typename V>
class HashTable
{
	private:
    	HashNode<K> **table;
    	fi * hash_function;
		int tableSize;

	public:
		HashTable(int tableSize, int k, int dim)
		{
			/*== constructing hash table*/
			this->table = new HashNode<K> *[tableSize]();

			this->tableSize = tableSize;

			/*== construct fi object, used for hashing*/
			this->hash_function = new fi(k, dim);
		}
	
		~HashTable()
		{
			/*== destroying all buckets*/
			HashNode<K> * temp = NULL;
			HashNode<K> * prev = NULL;

			for(int i=0; i<this->tableSize; i++)
			{
				temp = table[i];
				while(temp != NULL)
				{
					prev = temp;
					temp = temp->getNext();
					delete prev;
				}

				table[i] = NULL;
			}

			delete[] table;
		}
		
    	void put(const K &key)
		{
        	int hash_val = hash_function->hashValue(key, this->tableSize);
        	HashNode<K> *prev = NULL;
        	HashNode<K> *entry = table[hash_val];
			std::string G;

        	while (entry != NULL && entry->getKey() != key) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
				G = hash_function->computeG(key);
            	entry = new HashNode<K>(key, G);

            	if (prev == NULL) 
				{
                	/*== insert as first bucket*/
                	table[hash_val] = entry;
            	} 	
				else 
				{
               		prev->setNext(entry);
            	}
        	} 
    	}
    
//	bool get(const K &key, V &value) {
  //      unsigned long hashValue = hashFunc(key);
   //     HashNode<K, V> *entry = table[hashValue];
//
 //       while (entry != NULL) {
  //          if (entry->getKey() == key) {
   //             value = entry->getValue();
    //            return true;
     //       }
      //      entry = entry->getNext();
      //  }
     //   return false;
   // }
};

#endif

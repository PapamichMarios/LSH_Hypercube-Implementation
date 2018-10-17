#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <iostream>

#include "hash_node.h"
#include "fi.h"

template <typename K, typename V>
class HashTable
{
	private:
    	HashNode<K, V> **table;
    	fi * hash_function;
		int tableSize;

	public:
		HashTable(int tableSize, int k, int dim)
		{
			/*== constructing hash table*/
			this->table = new HashNode<K, V> *[tableSize]();

			this->tableSize = tableSize;

			/*== construct fi object, used for hashing*/
			this->hash_function = new fi(k, dim);
		}
	
		~HashTable()
		{
			/*== destroying all buckets*/
			HashNode<K, V> * temp = NULL;
			HashNode<K, V> * prev = NULL;

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
		
    	void put(const K &key, const V &value)
		{
        	int hash_val = hash_function->hashValue(key, this->tableSize);
        	HashNode<K, V> *prev = NULL;
        	HashNode<K, V> *entry = table[hash_val];

        	while (entry != NULL && entry->getKey() != key) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
            	entry = new HashNode<K, V>(key, value);
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

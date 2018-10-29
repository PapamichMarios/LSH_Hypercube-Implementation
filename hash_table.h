#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <string>
#include <algorithm>
#include <fstream>
#include <climits>
#include <ctime>
#include <map>

#include "hash_node.h"
#include "help_functions.h"
#include "fi.h"
#include "g.h"

template <typename K>
class HashTable
{
	protected:
    	HashNode<K> **table;
		int tableSize;

	public:
		HashTable(int tableSize, int k, int dim)
		{
			/*== constructing hash table*/
			this->table = new HashNode<K> *[tableSize]();

			this->tableSize = tableSize;
		}
	
		virtual ~HashTable()
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
		
    	virtual void put(const K &key, std::string identifier) =0;

		virtual std::vector<std::string> ANN(const K &query, double &distance_ANN, double & time_ANN) =0;
		virtual std::vector<std::string> NN(const K &query, double &distance_NN) =0;
		virtual void RS (const K &query, int c, double R, std::map<std::string, double> &dist_map) =0;
};

template <typename K>
class HashTable_EUC : public HashTable<K>
{
	private:
		fi * hash_function;

	public:	
		HashTable_EUC(int tableSize, int k, int dim) : HashTable<K>(tableSize, k, dim)
		{
			/*== construct fi object, used for hashing*/
			this->hash_function = new fi(k, dim);
		}

		~HashTable_EUC()
		{
			delete hash_function;
		}

    	void put(const K &key, std::string identifier)
		{
        	int hash_val = hash_function->hashValue(key, this->tableSize);
        	HashNode<K> *prev = NULL;
        	HashNode<K> *entry = this->table[hash_val];
			std::string G;

        	while (entry != NULL) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
				G = hash_function->computeG(key);
            	entry = new HashNode<K>(key, G, identifier);

            	if (prev == NULL) 
				{
                	/*== insert as first bucket*/
                	this->table[hash_val] = entry;
            	} 	
				else 
				{
               		prev->setNext(entry);
            	}
        	} 
    	}

		/*== Neighbour functions*/
		std::vector<std::string> NN(const K &query, double &distance_NN)
		{
			double distance;
			HashNode<K> * temp = NULL;
			clock_t end_time;

			double min_distance = INT_MAX;
			std::string identifier = "NONE";

			std::string G = hash_function->computeG(query);
			const clock_t begin_time = clock();

			/*== start iterating through the hash table*/
			for(int i=0; i<this->tableSize; i++)
			{
				/*== assign temp to the head of the bucket*/
				temp = this->table[i];	

				/*== iterate through every node in the bucket*/
				while( temp != NULL )
				{
					/*== compare the g(query) with the g(point)*/
					if( G != temp->getG() )
					{
						temp = temp->getNext();
						continue;
					}

					/*== if they match calculate distance*/
					distance = help_functions::euclidean_distance(query, temp->getKey());
					
					if(distance < min_distance)
					{
						min_distance = distance;
						identifier = temp->getId();
					}

					/*== continue the iteration*/
					temp = temp->getNext();
				}
			}
			
			end_time = clock();

			/*== save measurements into a vector of size 3
				 0: distance
				 1: identifier
				 2: time
			  == */

			if( min_distance < distance_NN )
				distance_NN = min_distance;

			std::vector<std::string> measurements(3);
			measurements[0] = std::to_string(min_distance);
			measurements[1] = identifier;
			measurements[2] = std::to_string(float(end_time - begin_time) / CLOCKS_PER_SEC);

			return measurements;
		}

		std::vector<std::string> ANN(const K &query, double &distance_ANN, double &time_ANN)
		{
			double min_distance = INT_MAX;
			std::string identifier = "NONE";
			double distance=0;
			clock_t end_time;

			int hash_val		= hash_function->hashValue(query, this->tableSize);
			std::string G		= hash_function->computeG(query);
			HashNode<K> * temp  = this->table[hash_val];
			const clock_t begin_time = clock();


			/*== get every point saved on the bucket and find the euclidean distance with query point*/
			while( temp != NULL )
			{
				/*== compare the g(query) with the g(point)*/
				if( G != temp->getG() )
				{
					temp = temp->getNext();
					continue;
				}

				/*== if they match, save the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
	
				if(distance < min_distance)
				{
					min_distance = distance;
					identifier = temp->getId();
				}

				/*== continue the iteration*/
				temp = temp->getNext();
			}
			end_time = clock();

			/*== save measurements into a vector of size 3
				 0: distance
				 1: identifier
				 2: time
			  == */

			if(min_distance < distance_ANN)
			{
				distance_ANN = min_distance;
				time_ANN = double(end_time - begin_time) / CLOCKS_PER_SEC;
			}

			std::vector<std::string> measurements(3);
			measurements[0] = std::to_string(min_distance);
			measurements[1] = identifier;
			measurements[2] = std::to_string(float(end_time - begin_time) / CLOCKS_PER_SEC);

			return measurements;
		}

		void RS(const K &query, int c, double R, std::map<std::string, double> &dist_map)
		{
			double distance=0;

			int hash_val		= hash_function->hashValue(query, this->tableSize);
			std::string G		= hash_function->computeG(query);
			HashNode<K> * temp  = this->table[hash_val];

			/*== iterate through the bucket*/
			while( temp != NULL )
			{
				/*== compare the g(query) with the g(point)*/
				if( G != temp->getG() )
				{
					temp = temp->getNext();
					continue;
				}
			
				/*== if they match calculate the distance*/
				distance = help_functions::euclidean_distance(query, temp->getKey());
	
				if(distance<c*R)
					dist_map[temp->getId()] = distance;

				/*== iterate to the next node*/
				temp = temp->getNext();	
			}	
		}

};

template <typename K>
class HashTable_COS : public HashTable<K>
{
	private:
		G * hash_function;

	public:
		HashTable_COS(int tableSize, int k, int dim) : HashTable<K>(tableSize, k, dim)
		{
			/*== construct g object, used for hashing*/
			this->hash_function = new G(k, dim);
		}

		~HashTable_COS()
		{
			delete hash_function;
		}

    	void put(const K &key, std::string identifier)
		{
        	int hash_val = hash_function->hashValue(key);
        	HashNode<K> *prev = NULL;
        	HashNode<K> *entry = this->table[hash_val];

        	while (entry != NULL) 
			{
            	prev = entry;
            	entry = entry->getNext();
        	}

        	if (entry == NULL) 
			{
            	entry = new HashNode<K>(key, std::to_string(hash_val), identifier);

            	if (prev == NULL) 
				{
                	/*== insert as first bucket*/
                	this->table[hash_val] = entry;
            	} 	
				else 
				{
               		prev->setNext(entry);
            	}
        	} 
			
		}

		/*== Neighbour functions*/
		std::vector<std::string> NN(const K &query, double &distance_NN)
		{
			double distance;
			HashNode<K> * temp = NULL;
			clock_t end_time;

			double min_distance = INT_MAX;
			std::string identifier = "NONE";
			const clock_t begin_time = clock();

			/*== start iterating through the hash table*/
			for(int i=0; i<this->tableSize; i++)
			{
				/*== assign temp to the head of the bucket*/
				temp = this->table[i];	
				
				/*== iterate through every node in the bucket*/
				while( temp != NULL )
				{
					/*== calculate distance*/
					distance = help_functions::cosine_distance(query, temp->getKey());
					
					if(distance < min_distance)
					{
						min_distance = distance;
						identifier = temp->getId();
					}

					/*== continue the iteration*/
					temp = temp->getNext();
				}
			}
			end_time = clock();

			/*== save measurements into a vector of size 3
				 0: distance
				 1: identifier
				 2: time
			  == */
			
			if(min_distance < distance_NN)
				distance_NN = min_distance;

			std::vector<std::string> measurements(3);
			measurements[0] = std::to_string(min_distance);
			measurements[1] = identifier;
			measurements[2] = std::to_string(float(end_time - begin_time) / CLOCKS_PER_SEC);

			return measurements;
		}

		std::vector<std::string> ANN(const K &query, double &distance_ANN, double &time_ANN)
		{
			double distance=0;
			double min_distance = INT_MAX;
			std::string identifier = "NONE";
			clock_t end_time;

			int hash_val		= hash_function->hashValue(query);
			HashNode<K> * temp  = this->table[hash_val];
			const clock_t begin_time = clock();

			/*== get every point saved on the bucket and find the euclidean distance with query point*/
			while( temp != NULL )
			{
				/*== calculate the distance*/
				distance = help_functions::cosine_distance(query, temp->getKey());
	
				if(distance < min_distance)
				{
					min_distance = distance;
					identifier = temp->getId();
				}

				/*== continue the iteration*/
				temp = temp->getNext();
			}
			end_time = clock();

			/*== save measurements into a vector of size 3
				 0: distance
				 1: identifier
				 2: time
			  == */

			if(min_distance < distance_ANN)
			{
				distance_ANN = min_distance;
				time_ANN = double(end_time - begin_time) / CLOCKS_PER_SEC;
			}

			std::vector<std::string> measurements(3);
			measurements[0] = std::to_string(min_distance);
			measurements[1] = identifier;
			measurements[2] = std::to_string(float(end_time - begin_time) / CLOCKS_PER_SEC);

			return measurements;
		}

		void RS(const K &query, int c, double R, std::map<std::string, double> &dist_map)
		{
			double distance=0;

			int hash_val		= hash_function->hashValue(query);
			HashNode<K> * temp  = this->table[hash_val];

			/*== iterate through the bucket*/
			while( temp != NULL )
			{
				/*== calculate the distance*/
				distance = help_functions::cosine_distance(query, temp->getKey());

				if(distance<c*R)
					dist_map[temp->getId()] = distance;

				/*== iterate to the next node*/
				temp = temp->getNext();	
			}	
		}
};

#endif

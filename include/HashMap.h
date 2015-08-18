#ifndef CLASS_NCINE_HASHMAP
#define CLASS_NCINE_HASHMAP

#include "Array.h"
#include "List.h"
#include "HashFunctions.h"

namespace ncine {

template <class K, class T, class HashFunc> class HashMapIterator;
class String;

template <class K, class T>
class HashMapNode
{
  public:
	hash_t hash;
	K key;
	T value;

	HashMapNode() : hash(0) { }
	HashMapNode(hash_t hh, const K& kk)
		: hash(hh), key(kk) { }
};

/// A template based hashmap implementation with separate chaining and list head cell
template <class K, class T, class HashFunc>
class HashMap
{
  public:
	explicit HashMap(unsigned int size);
	~HashMap() { clear(); }

	// Copy constructor
	HashMap(const HashMap& other);
	// Copy-and-swap assignment operator
	HashMap& operator=(HashMap other);

	/// Swaps two hashmaps without copying their data
	inline void swap(HashMap& first, HashMap& second)
	{
		nc::swap(first.buckets_, second.buckets_);
		nc::swap(first.hashFunc_, second.hashFunc_);
	}

	/// Subscript operator
	inline T& operator[](const K& key) { return retrieveBucket(key).findOrAdd(hashFunc_(key), key); }

	/// Returns the number of buckets
	inline unsigned int bucketAmount() const { return buckets_.size(); }
	/// Returns the size of the bucket for the hash generated by the specified key
	inline unsigned int bucketSize(const K& key) const { return retrieveBucket(key).size(); }
	/// Returns the hash of a given key
	inline hash_t hash(const K& key) const { return hashFunc_(key); }

	// Clears the hashmap
	void clear();
	/// Checks whether an element is in the hashmap or not
	inline bool contains(const K& key, T& returnedValue) const { return retrieveBucket(key).contains(hashFunc_(key), key, returnedValue); }
	/// Checks whether an element is in the hashmap or not
	/*! Prefer this method if copying T is expensive, but always check the validity of returned pointer */
	inline T* find(const K& key) { return retrieveBucket(key).find(hashFunc_(key), key); }
	// Removes a key from the hashmap, if it exists
	inline bool remove(const K& key) { return retrieveBucket(key).remove(hashFunc_(key), key); }

  private:
	/// The bucket class for the hashmap, with separate chaining and list head cell
	class HashBucket
	{
	  public:
		HashBucket() : size_(0) { }
		unsigned int size() const { return size_; }
		void clear();
		bool contains(hash_t hash, const K& key, T& returnedValue) const;
		T* find(hash_t hash, const K& key);
		T& findOrAdd(hash_t hash, const K& key);
		bool remove(hash_t hash, const K& key);

	  private:
		unsigned int size_;
		/// Separate chaining with head cell
		HashMapNode<K, T> firstNode_;
		/// Separate chaining with a linked list
		List<HashMapNode<K, T> > collisionList_;

		friend class HashMapIterator<K, T, HashFunc>;
	};

	const HashBucket& retrieveBucket(const K& key) const;
	HashBucket& retrieveBucket(const K& key);

	Array<HashBucket> buckets_;
	HashFunc hashFunc_;

	friend class HashMapIterator<K, T, HashFunc>;
};

template <class K, class T, class HashFunc>
void HashMap<K, T, HashFunc>::HashBucket::clear()
{
	collisionList_.clear();
	size_ = 0;
}

template <class K, class T, class HashFunc>
bool HashMap<K, T, HashFunc>::HashBucket::contains(hash_t hash, const K& key, T& returnedValue) const
{
	if (size_ == 0)
	{
		return false;
	}

	bool found = false;
	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		found = true;
		returnedValue = firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				found = true;
				returnedValue = (*i).value;
				break;
			}
		}
	}

	return found;
}

template <class K, class T, class HashFunc>
T* HashMap<K, T, HashFunc>::HashBucket::find(hash_t hash, const K& key)
{
	if (size_ == 0)
	{
		return NULL;
	}

	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		return &firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				return &(*i).value;
			}
		}
	}

	return NULL;
}

template <class K, class T, class HashFunc>
T& HashMap<K, T, HashFunc>::HashBucket::findOrAdd(hash_t hash, const K& key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		firstNode_.hash = hash;
		firstNode_.key = key;
		size_++;
		return firstNode_.value;
	}

	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		return firstNode_.value;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				return (*i).value;
			}
		}
	}

	// The item has not been found, a new entry is created at the end of the list
	size_++;
	collisionList_.insertBack(HashMapNode<K, T>(hash, key));
	return (*collisionList_.rBegin()).value;
}

/*!
	\return True if the element has been found and removed
*/
template <class K, class T, class HashFunc>
bool HashMap<K, T, HashFunc>::HashBucket::remove(hash_t hash, const K& key)
{
	if (size_ == 0)
	{
		// Early-out if the bucket is empty
		return false;
	}

	bool found = false;
	if (firstNode_.hash == hash && firstNode_.key == key)
	{
		// The item has been found in the direct access node
		found = true;

		// Bring the first element of the list, if any, as direct access node
		if (collisionList_.isEmpty() == false)
		{
			firstNode_ = collisionList_.removeFront();
		}
		size_--;
	}
	else
	{
		for (typename List<HashMapNode<K, T> >::Iterator i = collisionList_.begin(); i != collisionList_.end(); ++i)
		{
			if ((*i).hash == hash && (*i).key == key)
			{
				// The item has been found in the list
				found = true;
				collisionList_.remove(i++);
				size_--;

				// The item has been removed and the iterator is no longer valid
				break;
			}
		}
	}

	return found;
}

template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc>::HashMap(unsigned int size)
	: buckets_(size, true)
{
	for (unsigned int i = 0; i < size; i++)
	{
		buckets_[i] = HashBucket();
	}
}

/// Copy constructor
template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc>::HashMap(const HashMap<K, T, HashFunc>& other)
	: buckets_(other.buckets_), hashFunc_(other.hashFunc_)
{

}

/// Copy-and-swap assignment operator
/** The parameter should be passed by value for the idiom to work */
template <class K, class T, class HashFunc>
HashMap<K, T, HashFunc>& HashMap<K, T, HashFunc>::operator=(HashMap<K, T, HashFunc> other)
{
	swap(*this, other);
	return *this;
}

/// Clears the hashmap
template <class K, class T, class HashFunc>
void HashMap<K, T, HashFunc>::clear()
{
	for (unsigned int i = 0; i < buckets_.size(); i++)
	{
		buckets_[i].clear();
	}
}

template <class K, class T, class HashFunc>
inline const typename HashMap<K, T, HashFunc>::HashBucket& HashMap<K, T, HashFunc>::retrieveBucket(const K& key) const
{
	hash_t hash = hashFunc_(key);
	unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

template <class K, class T, class HashFunc>
inline typename HashMap<K, T, HashFunc>::HashBucket& HashMap<K, T, HashFunc>::retrieveBucket(const K& key)
{
	hash_t hash = hashFunc_(key);
	unsigned int index = hash % buckets_.size();
	return buckets_[index];
}

/// A way to simulate C++11 alias templates
template <class T>
struct StringHashMap
{
	typedef HashMap<String, T, SaxHashFunc<nc::String> > type;
};

}

#endif

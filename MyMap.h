#ifndef MY_MAP
#define MY_MAP


// MyMap.h

template <typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap()
	{
		m_size = 0;
		root = nullptr;
	}
	~MyMap()
	{
		clear();
	}
	void clear()
	{
		deleteTree(root);
		m_size = 0;
	}
	int size() const
	{
		return m_size;
	}
	void associate(const KeyType& key, const ValueType& value)
	{
		if (m_size == 0)
		{
			root = new Child(key, value);
			m_size++;
			return;
		}
		Child* it = root;
		while (it != nullptr)
			{
				if (key == it->m_key)
				{
					it->m_value = value;
					return;
				}
				if (key < (*it).m_key)
				{
					if (it->left == nullptr)
					{
						it->left = new Child(key, value);
						m_size++;
						return;
					}
					it = (*it).left;
				}
				else
				{
					if (it->right == nullptr)
					{
						it->right = new Child(key, value);
						m_size++;
						return;
					}
					it = (*it).right;
				}
			}
		
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const
	{
		Child* it = root;
		while (it != nullptr)
		{
			if (it->m_key == key)
			{
				ValueType* vp = &(it->m_value);
				return vp;
			}
			else if (key<it->m_key)
			{
				it = it->left;
			}
			else if (key>it->m_key)
			{
				it = it->right;
			}
		}
		return nullptr;
	}

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	int m_size;

	class Child
	{
	public:
		Child()
		{}
		Child(KeyType key,ValueType value) 
		{
			m_key = key;
			m_value = value;
			left = nullptr;
			right = nullptr;
		}
		Child* left;
		Child* right;
		KeyType m_key;
		ValueType m_value;
	};

	Child* root;

	void deleteTree(Child* cur)
	{
		if (cur == NULL)
			return;
		deleteTree(cur->left);
		deleteTree(cur->right);
		delete cur;
	}

};

#endif 

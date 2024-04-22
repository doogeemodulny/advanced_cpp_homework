#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

// Абстрактная реализация
template <typename T>
class SetImplementation {
public:
    virtual void addElement(T element) = 0;
    virtual void removeElement(T element) = 0;
    virtual bool containsElement(T element) const = 0;
    virtual int getSize() const = 0;
    virtual std::vector<T> getAsVector()=0;
    virtual void fillFromVector(std::vector<T>& dataVector) = 0;

    virtual SetImplementation<T>* unionSet(SetImplementation<T>* other)=0;
    virtual SetImplementation<T>* intersect(SetImplementation<T>* other)=0;
};

// Конкретная реализация для хранения множества в виде массива
template <typename T>
class VectorSet : public SetImplementation<T> {
private:
    std::vector<T> elements;
    int size;

public:
    VectorSet(){size=0;};
    void addElement(T element) override {
        elements.push_back(element);
        size+=1;
    }

    void removeElement(T element) override {
        auto it = std::find(elements.begin(), elements.end(), element);
        if (it != elements.end()) {
            elements.erase(it);
        }
        size-=1;
    }

    bool containsElement(T element) const override {
        return std::find(elements.begin(), elements.end(), element) != elements.end();
    }

    std::vector<T> getAsVector() override
    {
        return elements;
    };

    void fillFromVector(std::vector<T>& dataVector) override
    {
        elements=dataVector;
        size=dataVector.size();
    };

    int getSize() const override 
    {
        return size;
    }

    SetImplementation<T>* unionSet(SetImplementation<T>* other) override
    {
        SetImplementation<T>* result = new VectorSet<T>;
        std::vector<T> data = this->getAsVector();
        result->fillFromVector(data);
        for(auto elementFromOther: other->getAsVector())
        {
            if(!other->containsElement(elementFromOther))
                result->addElement(elementFromOther);
        }
        return result;
    };
    SetImplementation<T>* intersect(SetImplementation<T>* other) override 
    {
        SetImplementation<T>* result = new VectorSet<T>;
        for(auto element: this->getAsVector())
        {
            if(other->containsElement(element))
                result->addElement(element);
        }
        return result;
    };
};

// Конкретная реализация для хранения множества в виде Heap-дерева
template <typename T>
class TreapSet : public SetImplementation<T> {
private:
    struct Node {
        T key;
        int priority;
        Node* left;
        Node* right;
        
        Node(T k, int p) : key(k), priority(p), left(nullptr), right(nullptr) {}
    };

    Node* root;
    std::mt19937 rng; // Генератор случайных чисел
    int size;
    
    // Вспомогательная функция для вставки элемента в дерево
    Node* insertNode(Node* root, T key, int priority) {
        if (root == nullptr) {
            return new Node(key, priority);
        }
        
        if (key < root->key) {
            root->left = insertNode(root->left, key, priority);
            if (root->left->priority > root->priority) {
                // Поворот вправо
                Node* newRoot = root->left;
                root->left = newRoot->right;
                newRoot->right = root;
                return newRoot;
            }
        } else {
            root->right = insertNode(root->right, key, priority);
            if (root->right->priority > root->priority) {
                // Поворот влево
                Node* newRoot = root->right;
                root->right = newRoot->left;
                newRoot->left = root;
                return newRoot;
            }
        }
        
        return root;
    }
    
    // Вспомогательная функция для удаления элемента из дерева
    Node* removeNode(Node* root, T key) {
        if (root == nullptr) {
            return root;
        }
        
        if (key < root->key) {
            root->left = removeNode(root->left, key);
        } else if (key > root->key) {
            root->right = removeNode(root->right, key);
        } else {
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            
            // Выполняем поворот для удаления узла с двумя детьми
            if (root->left->priority > root->right->priority) {
                // Поворот вправо
                Node* newRoot = root->left;
                root->left = newRoot->right;
                newRoot->right = root;
                root = newRoot;
            } else {
                // Поворот влево
                Node* newRoot = root->right;
                root->right = newRoot->left;
                newRoot->left = root;
                root = newRoot;
            }
            
            // Рекурсивно удаляем узел
            root = removeNode(root, key);
        }
        
        return root;
    }
    
    // Вспомогательная функция для проверки наличия элемента в дереве
    bool containsNode(Node* root, T key) const {
        if (root == nullptr) {
            return false;
        }
        if (key < root->key) {
            return containsNode(root->left, key);
        } else if (key > root->key) {
            return containsNode(root->right, key);
        } else {
            return true;
        }
    }
    
    // Вспомогательная функция для обхода дерева и добавления элементов в std::vector
    void inorderTraversal(Node* node, std::vector<T>& result) {
        if (node != nullptr) {
            inorderTraversal(node->left, result);
            result.push_back(node->key);
            inorderTraversal(node->right, result);
        }
    }
    
public:
    TreapSet() : root(nullptr), rng(std::random_device()()) {size=0;}
    
    void addElement(T element) override {
        std::cout<<"i am in treapset add element\n";
        int priority = rng();
        root = insertNode(root, element, priority);
        size+=1;
    }
    
    void removeElement(T element) override {
        root = removeNode(root, element);
        size-=1;
    }
    
    bool containsElement(T element) const override {
        return containsNode(root, element);
    }
    
    // Метод для заполнения дерева из std::vector
    void fillFromVector(std::vector<T>& vec) override{
        std::cout<<"trying to fill from vector\n";
        for (auto element : vec) {
            addElement(element);
        }
        size=vec.size();
    }
    
    // Метод для получения содержимого дерева в виде std::vector
    std::vector<T> getAsVector() override{
        std::vector<T> result;
        inorderTraversal(root, result);
        return result;
    }

    int getSize() const override
    {
        return size;
    }

    SetImplementation<T>* unionSet(SetImplementation<T>* other) override
    {
        SetImplementation<T>* result = new TreapSet<T>;
        std::vector<T> data = this->getAsVector();
        std::cout<<"here";
        result->fillFromVector(data);
        for(auto elementFromOther: other->getAsVector())
        {
            if(!other->containsElement(elementFromOther))
                result->addElement(elementFromOther);
        }
        return result;
    };
    SetImplementation<T>* intersect(SetImplementation<T>* other) override
    {
        SetImplementation<T>* result = new TreapSet<T>;
        for(auto element: this->getAsVector())
        {
            if(other->containsElement(element))
                result->addElement(element);
        }
        return result;
    };
};

// Абстракция
template <typename T>
class Set {
protected:
    SetImplementation<T>* implementation;
    const int sizeToChangeImplementation;
    void changeImplementation() 
    {
        std::cout<<"I am about to change implementation\n";
        if (auto vectorImplementation = dynamic_cast<VectorSet<T>*>(implementation))
        {
            std::cout<<"now impl will be changed to treap\n";
            SetImplementation<T>* treapSet = new TreapSet<T>;
            std::vector<T> dataFromVector = vectorImplementation->getAsVector();
            treapSet->fillFromVector(dataFromVector);
            implementation=treapSet;
            std::cout<<"impl changed to treap\n";
        }
        else if (auto treapImplementation = dynamic_cast<TreapSet<T>*>(implementation))
        {
            std::cout<<"now impl will be changed to vector\n";
            SetImplementation<T>* vectorSet = new VectorSet<T>;
            auto dataFromVector = treapImplementation->getAsVector();
            vectorSet->fillFromVector(dataFromVector);
            implementation=vectorSet;
        }
    }

public:
    Set(SetImplementation<T>* impl, int sizeToChangeImplementation) : implementation(impl), sizeToChangeImplementation(sizeToChangeImplementation){}
    virtual void addElement(T element) {
        implementation->addElement(element);
        if(implementation->getSize()==sizeToChangeImplementation)
            changeImplementation();
    }
    virtual void removeElement(T element) {
        implementation->removeElement(element);
        if(implementation->getSize()==sizeToChangeImplementation-1)
            changeImplementation();
    }
    virtual bool containsElement(T element) const {
        return implementation->containsElement(element);
    }

    Set unionSet(Set<T>* other)
    {
        Set<T>* result = new Set<T>(implementation->unionSet(other->implementation), sizeToChangeImplementation);
        return *result;
    }

    Set intersectSet(Set* other)
    {
        Set<T>* result = new Set<T>(implementation->intersect(other->implementation), sizeToChangeImplementation);
        return *result;
    }
};

int main() {
    VectorSet<int> vectorSet;
    Set<int> set1(&vectorSet, 4);

    set1.addElement(5);
    set1.addElement(10);
    set1.addElement(15);
    set1.addElement(20);
    set1.addElement(25);
    std::cout<<"is 5 in set? "<<set1.containsElement(5)<<"\n";
    set1.removeElement(5);
    std::cout<<"is 5 in set? "<< set1.containsElement(5)<<"\n";
    set1.removeElement(25);
    set1.removeElement(15);
    set1.addElement(1230);
    set1.addElement(228);
    set1.addElement(345);
    
    VectorSet<int> vectorSet2;
    Set<int> set2(&vectorSet2, 10);
    set2.addElement(345);
    set2.addElement(228);
    set2.addElement(985);

    Set set12 = set1.unionSet(&set2);
    Set set1_2 = set1.intersectSet(&set2);
    std::cout<<"is 1230 in set1 union set2? "<<set12.containsElement(1230)<<"\n";
    std::cout<<"is 345 in set1 intersect set2? "<<set1_2.containsElement(345)<<"\n";
    std::cout<<"is 1230 in set1 intersect set2? "<<set1_2.containsElement(1230)<<"\n";
    return 0;
}

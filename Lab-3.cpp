#include <iostream>
#include <fstream>

using namespace std;
const int matrix_size = 100;
const char* PATH = "Some way";

struct node 
{
    int key = 0;
    int val = 0;
    node* next = nullptr;

    node(int v, int i)
    {
        key = i;
        val = v;
    }
};

struct list 
{
    int size = 0;
    node* head = nullptr;
    node* flow_head = nullptr;
    void add(int val, int key)
    {
        if (head == nullptr) head = new node(val, key);
        else
        {
            flow_head = head;
            while (flow_head->next != nullptr)
            {
                flow_head = flow_head->next;
            }
            flow_head->next = new node(val, key);
        }
        size++;
    }

    int find_key(int key)
    {
        flow_head = head;
        while (flow_head != nullptr && flow_head->key != key) 
            flow_head = flow_head->next;

        if (flow_head != nullptr) 
            return flow_head->val;

        return 0;
    }

    node* find_index(int index) 
    {
        if (index >= size) 
            return nullptr;

        low_head = head;

        for (int i = 0; i < index; i++) 
            low_head = low_head->next;
        return local_head;

    }

    void clear() 
    {
        head = nullptr;
        low_head = nullptr;
        size = 0;
    }

    void print_to() 
    {
        low_head = head;
        while (low_head != nullptr)
        {
            cout << "key = " << low_head->key << "\tvalue = " << low_head->val << endl;
            low_head = low_head->next;
        }
    }
};

int* matrix_to_array(int** matrix)
{
    int* arr = new int[matrix_size * matrix_size];

    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++) 
        {
            arr[i * matrix_size + j] = matrix[i][j];
        }
    }
    return arr;
}

list* matrix_transform(int** matrix)
{
    list* arr = new list;

    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            if (matrix[i][j] != 0) 
                arr->add(matrix[i][j], i * matrix_size + j);
        }
    }
    return arr;
}

int** list_convert_back(list* l)
{
    int** matrix = new int* [matrix_size];

    for (int i = 0; i < matrix_size; i++) 
    {
        matrix[i] = new int[matrix_size];
        for (int j = 0; j < matrix_size; j++)
        {
            matrix[i][j] = l->find_key(i * matrix_size + j);
        }
    }
    return matrix;
}

list* matrix_index(int** matrix)
{
    int* arr = matrix_to_array(matrix);
    list* l = new list;
    int counter = 1;

    for (int i = 0; i < matrix_size * matrix_size; i++) 
    {
        if (i != 0)
        {
            if (arr[i] == arr[i - 1])
            {
                counter++;
            }
            else
            {
                l->add(counter, arr[i - 1]);
                counter = 1;
            }
        }
    }
    l->add(counter, arr[matrix_size * matrix_size - 1]);

    return l;
}

int** list_unindex(list* l) 
{
    int** matrix = new int* [matrix_size];
    matrix[0] = new int[matrix_size];
    int i = 0;
    int j = 0;
    int index = 0;
    node* n = l->find_index(index);

    while (n != nullptr)
    {
        for (int k = 0; k < n->val; k++)
        {
            matrix[i][j] = n->key;

            if (j + 1 == matrix_size)
            {
                i++;
                j = 0;
                matrix[i] = new int[matrix_size];
            }
            else j++;
        }
        index++;
        n = l->find_index(index);
    }
    return matrix;
}

int** add(int** matrix, int** m)
{
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            matrix[i][j] += m[i][j];
        }
    }
    return matrix;
}

int** convers(int** matrix, int** m)
{
    int** new_matrix = new int* [matrix_size];
    int counter;

    for (int i = 0; i < matrix_size; i++) 
    {
        new_matrix[i] = new int[matrix_size];

        for (int j = 0; j < matrix_size; j++)
        {
            counter = 0;
            for (int k = 0; k < matrix_size; k++) 
            {
                counter += matrix[k][j] * m[i][k];
            }
            new_matrix[i][j] = counter;
        }
    }
    return new_matrix;
}

void write(list* l, const char* path)
{
    fstream file(path, ios::binary | ios::out);
    int key = 0;
    int val = 0;

    for (int i = 0; i < l->size; i++) 
    {
        key = l->find_index(i)->key;
        val = l->find_index(i)->val;

        cout << key << " " << val << endl;

        file.write((char*)&key, sizeof(int));
        file.write((char*)&val, sizeof(int));
    }
    file.close();
}

void read(list* l, const char* path) 
{
    fstream file(path, ios::binary | ios::in);
    int val = 0;
    int key = 0;

    while (1) 
    {
        file.read((char*)&key, sizeof(int));
        if (file.eof()) 
            break;
        file.read((char*)&val, sizeof(int));
        l->add(val, key);
    }
    file.close();
}

void print(int** matrix) 
{
    for (int j = 0; j < matrix_size; j++)
    {
        for (int i = 0; i < matrix_size; i++)
        {
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
}

int main()
{
    int** matrix = new int* [matrix_size];
    int** n = new int* [matrix_size];

    for (int i = 0; i < matrix_size; i++)
    {
        matrix[i] = new int[matrix_size];
        n[i] = new int[matrix_size];
        for (int j = 0; j < matrix_size; j++)
        {
            matrix[i][j] = i * (j + 2);
            n[i][j] = -i * (j + 2);
        }
    }
    print(matrix);
    list* l = matrix_transform(matrix);
    write(l, PATH);
    l->clear();
    read(l, PATH);
    l->print_to();
    matrix = list_convert_back(l);
    print(matrix);
    return 0;
}
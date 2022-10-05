#include <iostream>
#include <string>

using namespace std;

/* Node structure for columns in increased order,
 * stored in same double linked list if nodes are in the same row
 */
typedef struct _Node
{
    long long col;
    _Node *next;
    _Node *prev;
    _Node (long long col, struct _Node *next, struct _Node *prev): col(col), next(next), prev(prev){};
} Node;

/* Double linked list structure for rows in increased order */
typedef struct _LinkedList 
{
    long long count;
    long long row;
    Node *head;
    Node *tail;
    _LinkedList *next;
    _LinkedList *prev;
    _LinkedList (long long row, Node *node, struct _LinkedList *next, struct _LinkedList *prev): count(1), row(row), head(node), tail(node), next(next), prev(prev){};
} LinkedList;

/* House Command:
 * 1. Directly create a new list if empty
 * 2. Find the linked list (row) we should insert at / before it
 *    a. If the row already exist, insert in increase order
 *    b. Otherwise, create one and insert before the position of cursor
 *    c. Always remember to update the next/prev connections of each node
 */
void house(LinkedList **head, LinkedList **tail, long long col, long long row)
{
    // If empty
    if (*head == NULL) {
        Node *new_node = new Node(col, NULL, NULL);
        *head = *tail = new LinkedList(row, new_node, NULL, NULL);
        return;
    }

    // Find row
    LinkedList **cursor = head;
    while (*cursor) {
        if ((*cursor)->row >= row)
            break;
        cursor = &(*cursor)->next;
    }

    // The largest row of existing ones, append at the end of the lists
    if (*cursor == NULL) {
        Node *new_node = new Node(col, NULL, NULL);
        LinkedList *new_list = new LinkedList(row, new_node, NULL, *tail);
        *cursor = *tail = new_list;
        return;
    }

    // Insert into current row
    if ((*cursor)->row == row) {
        Node **cursor_n = &(*cursor)->head;
        while (*cursor_n) {
            if ((*cursor_n)->col >= col)
                break;
            cursor_n = &(*cursor_n)->next;
        }

        // Largest node in the row, append at the end of the list
        if (*cursor_n == NULL) {
            Node *new_node = new Node(col, NULL, (*cursor)->tail);
            *cursor_n = new_node;
            (*cursor)->tail = new_node;
            (*cursor)->count++;
            return;
        }

        // If duplicated, stop
        if ((*cursor_n)->col == col)
            return;

        // Insert new node at middle/start of the list
        Node *new_node = new Node(col, *cursor_n, (*cursor_n)->prev);
        (*cursor_n)->prev = new_node;
        *cursor_n = new_node;
        (*cursor)->count++;

    // Create a new list, insert into middle/start of lists
    } else {
        Node *new_node = new Node(col, NULL, NULL);
        LinkedList *new_list = new LinkedList(row, new_node, *cursor, (*cursor)->prev);
        (*cursor)->prev = new_list;
        *cursor = new_list;
    }
    return;
}

/* Bomb Command:
 * 1. Stop if empty
 * 2. Find the linked list (row) where we need to find the targets
 *    a. row: find [col - 1, col + 1] and delete them
 *    b. row - 1: find col and delete it
 *    c. row + 1: find col and delete it
 *    d. delete whole row if it becomes empty after deleting nodes
 */
void bomb(LinkedList **head, LinkedList **tail, long long col, long long row)
{
    // If empty, stop
    if (*head == NULL)
        return;

    // Find row
    LinkedList **cursor = head;
    while (*cursor) {
        // If exceed row+1, stop
        if ((*cursor)->row > row + 1)
            break;
        // row-1 / row+1 found
        if ((*cursor)->row == row - 1 || (*cursor)->row == row + 1) {
            // Find col
            Node **cursor_n = &(*cursor)->head;
            while (*cursor_n) {
                // If exceed col, stop
                if ((*cursor_n)->col > col)
                    break;
                // col found
                if ((*cursor_n)->col == col) {
                    Node *next = (*cursor_n)->next;
                    if (next != NULL)
                        next->prev = (*cursor_n)->prev;
                    if (*cursor_n == (*cursor)->tail)
                        (*cursor)->tail = (*cursor_n)->prev;
                    delete *cursor_n;
                    *cursor_n = next;
                    (*cursor)->count--;
                } else {
                    cursor_n = &(*cursor_n)->next;
                }
            }
            // Check if empty, if so, delete whole linked list
            if ((*cursor)->count == 0) {
                LinkedList *next = (*cursor)->next;
                if (next != NULL)
                    next->prev = (*cursor)->prev;
                if (*cursor == *tail)
                    *tail = (*cursor)->prev;
                delete *cursor;
                *cursor = next;
            } else {
                cursor = &(*cursor)->next;
            }
        // Find row
        } else if ((*cursor)->row == row) {
            Node **cursor_n = &(*cursor)->head;
            while (*cursor_n) {
                // If exceed col + 1, stop
                if ((*cursor_n)->col > col + 1)
                    break;
                // [col - 1, col + 1] found
                if ((*cursor_n)->col >= col - 1 && (*cursor_n)->col <= col + 1) {
                    Node *next = (*cursor_n)->next;
                    if (next != NULL)
                        next->prev = (*cursor_n)->prev;
                    if (*cursor_n == (*cursor)->tail)
                        (*cursor)->tail = (*cursor_n)->prev;
                    delete *cursor_n;
                    *cursor_n = next;
                    (*cursor)->count--;
                } else {
                    cursor_n = &(*cursor_n)->next;
                }
            }
            // Check if empty, if so, delete whole linked list
            if ((*cursor)->count == 0) {
                LinkedList *next = (*cursor)->next;
                if (next != NULL)
                    next->prev = (*cursor)->prev;
                if (*cursor == *tail)
                    *tail = (*cursor)->prev;
                delete *cursor;
                *cursor = next;
            } else {
                cursor = &(*cursor)->next;
            }
        } else {
            cursor = &(*cursor)->next;
        }
    }
    return;
}

/* PrintFront Command:
 * 1. Use the head pointer of linked lists as the starter
 * 2. Use each head pointer in linked lists as starters of rows
 */
void printFront(LinkedList *head)
{
    cout << "PrintFront\n";
    if (head == NULL)
        return;

    LinkedList *cursor = head;
    while (cursor) {
        Node *cursor_n = cursor->head;
        while (cursor_n) {
            cout << "(" << cursor_n->col << "," << cursor->row << ")\n";
            cursor_n = cursor_n->next;
        }
        cursor = cursor->next;
    }
    return;
}

/* PrintEnd Command:
 * 1. Use the tail pointer of linked lists as the starter backward
 * 2. Use each tail pointer in linked lists as starters of rows backward
 */
void printEnd(LinkedList *tail)
{
    cout << "PrintEnd\n";
    if (tail == NULL)
        return;

    // Find largest row
    LinkedList *cursor = tail;
    while (cursor) {
        Node *cursor_n = cursor->tail;
        while (cursor_n) {
            cout << "(" << cursor_n->col << "," << cursor->row << ")\n";
            cursor_n = cursor_n->prev;
        }
        cursor = cursor->prev;
    }
    return;
}

int main()
{
    LinkedList *head = NULL;
    LinkedList *tail = NULL;

    long long count;
    cin >> count;
    long long col, row;
    string command;
    for(long long i = 0; i < count; i++){
        cin >> command;
        if(command.compare("House") == 0){
            cin >> col >> row;
            house(&head, &tail, col, row);
        }
        else if(command.compare("Bomb") == 0){
            cin >> col >> row;
            bomb(&head, &tail, col, row);
        }
        else if(command.compare("PrintFront") == 0){
            printFront(head);
        }
        else if(command.compare("PrintEnd") == 0){
            printEnd(tail);
        }       
    }
}
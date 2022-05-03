search_node( NODE *, int)
    find the node that has the key.
*/
NODE* search_node( NODE *t, int key)
{
    while( t != NULL && t->key != key)
    {
        if( t->key > key)
        {
            t   = t->left;
        }
        else
        {
            t   = t->right;
        }
    }

    return t;
}
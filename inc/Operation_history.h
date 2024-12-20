//
// Created by paa04 on 12.12.24.
//

#ifndef OPERATION_HISTORY_H
#define OPERATION_HISTORY_H

#include <vector>

#include "Operation.h"

using namespace std;

class Operation_history
{
private:
    vector<shared_ptr<Operation>> history_;
    int op_index = 0;

public:
    void insert_op(shared_ptr<Operation> op);
    shared_ptr<Operation> restore_op();
    shared_ptr<Operation> repeat_op();

    vector<shared_ptr<Operation>> actual_ops();

    void clear();
};


#endif //OPERATION_HISTORY_H

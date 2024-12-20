//
// Created by paa04 on 12.12.24.
//

#include "Operation_history.h"

void Operation_history::insert_op(shared_ptr<Operation> op)
{
    history_.erase(history_.begin() + op_index, history_.end());

    history_.push_back(op);
    op_index++;
}


shared_ptr<Operation> Operation_history::restore_op()
{
    if (op_index > 0)
        return history_[--op_index];
    else
        return nullptr;
}

shared_ptr<Operation> Operation_history::repeat_op()
{
    if(op_index < history_.size())
    {
        return history_[op_index++];
    }

    return nullptr;
}

void Operation_history::clear()
{
    op_index = 0;
    history_.clear();
}

vector<shared_ptr<Operation> > Operation_history::actual_ops()
{
    auto v = history_;

    v.erase(v.begin() + op_index, v.end());

    return v;
}

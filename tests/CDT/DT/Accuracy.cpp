#include "Accuracy.h"

#include<cassert>

double accuracy(const y& y_test, const y& y_pred) {
    assert(y_pred.size() == y_test.size());
    double accuracy = 0;
    for (size_t i = 0; i < y_pred.size(); i++) {
        if (y_pred[i] == y_test[i]) { accuracy += 1; }
    }
    return accuracy / y_pred.size();
}
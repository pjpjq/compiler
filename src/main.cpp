#include <iostream>
#include <cassert>
#include <string>

#include "Lexer.h"

std::string buffer;
int cur_idx = 0;

bool analyse_A();

bool analyse_B();

/* ����ǰ����, ��++idx */
char peek_symbol() {
    assert(cur_idx >= 0);

//    if (cur_idx < 0) {
//        std::cout << "Index out of range! cur_idx = " << cur_idx << std::endl;
//        return -1;
//    }
    
    if (cur_idx >= buffer.size()) {
        std::cout << "ERROR: Index out of range! cur_idx = " << cur_idx << std::endl;
        return 0;
    }
    return buffer[cur_idx];
}

/* ����ǰ�Ƿ�β�� */
bool reach_end() {
    return cur_idx == buffer.size();
}

/* ����ǰ����, ��׼�����¸����ŵ� idx �� ++idx */
char fetch_symbol() {
    char symbol = peek_symbol();
    ++cur_idx;
    return symbol;
}

/* B -> a[A] */
bool analyse_B() {
    if (fetch_symbol() != 'a') {
        return false;
    }
    if (peek_symbol() == 'c') {
        /* Use backtrack here... */
        int backtrack_idx = cur_idx;
        if (!analyse_A()) {
            cur_idx = backtrack_idx;
        }
    }
    return true;
}

/* A -> c{aB} */
bool analyse_A() {
    char symbol = fetch_symbol();
    if (symbol == 'c') {
        while (peek_symbol() == 'a') { /* ȡ������ aB */
            fetch_symbol();
            if (!analyse_B()) {
                return false;
            }
        }
        return true;
    }
    return false;
}

/* Z -> c{aB}cB | a[A]d */
bool analyse_Z() {
    char symbol = fetch_symbol();
    if (symbol == 'c') {  /* {aB}cB */
        while (peek_symbol() == 'a') { /* ȡ������ aB */
            fetch_symbol();
            if (!analyse_B()) {
                return false;
            }
        }
        return fetch_symbol() == 'c' && analyse_B() && reach_end();  /* cB */
    } else if (symbol == 'a') {  /* [A]d */
        return (peek_symbol() != 'c' || analyse_A()) && fetch_symbol() == 'd' && reach_end();
    }
    return false;
}

/* G[Z] */
bool is_valid_G() {
    return analyse_Z();
}

/* Clears buffer and resets current index. */
void reset_buffer() {
    cur_idx = 0;
    buffer.clear();
}

int main() {
    /* ���� n_tests �� */
    int n_tests = 10;
    for (int i = 0; i < n_tests; ++i) {
        reset_buffer();
        std::cin >> buffer;
        std::cout << is_valid_G() << std::endl;
    }
    return 0;
}
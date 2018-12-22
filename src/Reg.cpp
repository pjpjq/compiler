////
//// Created by monotasker on 2018/12/16.
////
//
//#include "Reg.h"
//
//int cur_use_count = 0;
//
//void Reg::init() {
//    if (content2reg.count(content_name) > 0) {
//        content2reg.erase(content2reg.find(content_name));
//    }
//    content_name.clear();
//    state = FREE;
//}
//
//void Reg::store() {
//    if (state == DIRTY) {
//        std::string ptr = is_global ? "($gp)" : "($fp)";
//        std::cout.setf(std::ios::left);
//        code_fout << "sw " << reg_name << ", " << offset << ptr << std::setw(45) << "#\tstore " << content_name
//                  << std::endl;
//    }
//}
//
//void Reg::load() {
//    std::cout.setf(std::ios::left);
//    if (is_string_number(content_name)) {
//        code_fout << "li " << reg_name << ", " << content_name;
//    } else if (offset != -1) {
//        std::string ptr = is_global ? "($gp)" : "($fp)";
//        std::cout.setf(std::ios::left);
//        code_fout << "lw " << reg_name << ", " << offset << ptr << std::setw(45) << "#\tload " << content_name
//                  << std::endl;
//    }
//}
//
//void Reg::write_back() {
//    store();
//    init();
//    n_uses = cur_use_count++;
//}

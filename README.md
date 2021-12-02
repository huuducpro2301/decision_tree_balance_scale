# decision_tree_balance_scale
1. Giới thiệu input/output
  - file "train.txt" (439 dòng) dùng để xây dựng decision tree 
  - file "valid.txt" (93 dòng) dùng để đánh giá các cây train được
  - file "private_test.txt" (93 dòng) là bộ hidden test 
  - file "tree.txt" là output biểu diễn cây tốt nhất xây dựng được
  - file "result_details.txt" là output về kết quả chi tiết của các cây, bao gồm train accuracy rate và valid accuracy rate
2. Thuật toán xây dựng
  - Xây dựng cây đệ quy từ gốc, với mỗi node chọn cách chia tốt nhất tại node đó (theo tiêu chí gini hoặc entropy)
  - Sử dụng 2 tham số min size, max depth để làm trường hợp cơ sở (giới hạn) cho quá trình đệ quy
  - Sử dụng tập validation từ file "valid.txt" để đánh giá các cây xây được từ các min size, max depth khác nhau
3. 

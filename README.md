# decision_tree_balance_scale
1. Giới thiệu input/output
  - file "train.txt" (439 dòng) dùng để xây dựng decision tree 
  - file "valid.txt" (93 dòng) dùng để đánh giá các cây train được
  - file "private_test.txt" (93 dòng) là bộ hidden test 
  - file "tree.txt" là output biểu diễn cây tốt nhất xây dựng được
  - file "result_details.txt" là output về kết quả chi tiết của các cây, bao gồm train accuracy rate và valid accuracy rate
  - file test result
2. Thuật toán xây dựng
  - Xây dựng cây đệ quy từ gốc, với mỗi node chọn cách chia tốt nhất tại node đó (theo tiêu chí gini hoặc entropy)
  - Sử dụng 2 tham số min size, max depth để làm trường hợp cơ sở (giới hạn) cho quá trình đệ quy
  - Sử dụng tập validation từ file "valid.txt" để đánh giá các cây xây được từ các min size, max depth khác nhau
3. Giới thiệu cấu trúc
  - vector <vector <int> > train,valid,test:  bộ dữ liệu train, validation, hidden test
  - void get_train_data, get_valid_test, get_hidden_test: hàm get dữ liệu
  - void train_decision_tree_classcifier(): hàm chính, xây dựng cây theo dữ liệu đã có
  - double entropy(vector <int> dataset): độ nhiễu tập dữ liệu dataset theo tiêu chí entropy
  - double gini(vector <int> dataset): độ nhiễu tập dữ liệu dataset theo tiêu chí gini
  - string major_label(vector <int> dataset): nhãn chính của tập dữ liệu 
  - void split(node *T,vector <int> dataset,int max_depth,int min_size,string criterion): chia tập dữ liệu theo tiêu chí criterion 
  với min_size, max_depth là giới hạn về kích thước tối đa của node và độ sâu max của cây
  - string predict(vector <int> datapoint): nhãn dự đoán của cây quyết định về dữ liệu datapoint
  - int get_train_corrects(), get_valid_corects(): hàm trả về số dữ liệu dự đoán chính xác của cây hiện tại
  - void export_tree() -> export_node(node *t): in cây vào "tree.txt"
  

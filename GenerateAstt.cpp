#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm> 
#include <cctype>

std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}
struct AstType{
    std::string baseName;
    std::string className;
    std::vector<std::pair<std::string,std::string>> fields;
};

void defineAst(const std::string& outputDir,const std::string &baseName,const std::vector<AstType>& types){
    std::string path = outputDir+"/"+ baseName+".h";
    std::ofstream writer(path);
    if(!writer){
        std::cerr<<"Failed to create file:"<<path<<std::endl;
        return;
    }
    // 头文件
    writer << "#ifndef " << baseName << "_H\n";
    writer << "#define " << baseName << "_H\n\n";
    
    writer << "#include <memory>\n";
    writer << "#include <vector>\n";
    writer << "#include <any>\n";
    writer << "#include \"Token.h\"\n\n";
    
    for (const auto& type : types) {
        writer << "class " << type.className << ";\n";
    }
    writer << "\n";
    
    // Visitor 
    writer << "// Visitor \n";
    writer << "class " << baseName << "Visitor {\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "Visitor() = default;\n\n";
    
    for (const auto& type : types) {
        writer << "    virtual std::any visit" << type.className << baseName 
               << "(std::shared_ptr<" << type.className << "> " << toLower(baseName) << ") = 0;\n";
    }
    writer << "};\n\n";
    
    writer << "class " << baseName << " {\n";
    writer << "public:\n";
    writer << "    virtual ~" << baseName << "() = default;\n";
    writer << "    virtual std::any accept(" << baseName << "Visitor& visitor) = 0;\n";
    writer << "};\n\n";
    
    // 生成各个子类
    for (const auto& type : types) {
        writer << "class " << type.className << " : public " << baseName << ",\n";
        writer << "                                 public std::enable_shared_from_this<" << type.className << "> {\n";
        writer << "public:\n";
        
        // 构造函数
        writer << "    " << type.className << "(";
        for (size_t i = 0; i < type.fields.size(); ++i) {
            writer << type.fields[i].first << " " << type.fields[i].second;
            if (i < type.fields.size() - 1) writer << ", ";
        }
        writer << ")\n";
        
        // 初始化列表
        if (!type.fields.empty()) {
            writer << "        : ";
            for (size_t i = 0; i < type.fields.size(); ++i) {
                writer << type.fields[i].second << "(" << type.fields[i].second << ")";
                if (i < type.fields.size() - 1) writer << ", ";
            }
        }
        writer << " {}\n\n";
        
        // 字段
        writer << "    // 字段\n";
        for (const auto& field : type.fields) {
            writer << "    " << field.first << " " << field.second << ";\n";
        }
        writer << "\n";
        
        // accept 方法
        writer << "    std::any accept(" << baseName << "Visitor& visitor) override {\n";
        writer << "        return visitor.visit" << type.className << baseName 
               << "(shared_from_this());\n";
        writer << "    }\n";
        
        writer << "};\n\n";
    }
    
    writer << "#endif // " << baseName << "_H\n";
    writer.close();
    
    std::cout << "Generated: " << path << std::endl;
}

int main(int argc,char* argv[]){
    if(argc!=2){
        std::cerr<<"Usage: generate_ast <output directory>"<<std::endl;
        return 64;
    }

    std::string outputDir =argv[1];

    std::vector<AstType> exprTypes ={
        {"Expr","Binary",{{"Expr*","left"},{"Token","op"},{"Expr*","right"}}},
        {"Expr","Grouping",{{"Expr*","expression"}}},
        {"Expr","Literal",{{"std::any","value"}}},
        {"Expr","Unary",{{"Token","op"},{"Expr*","right"}}}
    };
    defineAst(outputDir,"Expr",exprTypes);
    return 0;
}
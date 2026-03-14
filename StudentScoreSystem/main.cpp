#include <iostream>
#include <sqlite3.h>  // 如果能找到这个文件，说明配置成功

int main() {
    sqlite3* db;
    int rc = sqlite3_open("test.db", &db);

    if (rc) {
        std::cerr << "数据库打开失败: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    std::cout << "恭喜！SQLite配置成功！" << std::endl;
    std::cout << "SQLite版本: " << sqlite3_libversion() << std::endl;

    // 创建一个测试表
    const char* sql = "CREATE TABLE IF NOT EXISTS test(id INT, name TEXT);";
    char* errMsg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL错误: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "数据表创建成功！" << std::endl;
    }

    // 插入一条数据
    sql = "INSERT INTO test VALUES (1, '测试数据');";
    rc = sqlite3_exec(db, sql, 0, 0, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "插入失败: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
    else {
        std::cout << "数据插入成功！" << std::endl;
    }

    // 查询数据
    sql = "SELECT * FROM test;";
    std::cout << "\n查询结果：" << std::endl;
    sqlite3_exec(db, sql,
        [](void* data, int argc, char** argv, char** colName) {
            for (int i = 0; i < argc; i++) {
                std::cout << colName[i] << " = " << (argv[i] ? argv[i] : "NULL") << " ";
            }
            std::cout << std::endl;
            return 0;
        }, nullptr, &errMsg);

    sqlite3_close(db);

    // 检查test.db文件是否生成
    FILE* file;
    errno_t err = fopen_s(&file, "filename.txt", "r");
    if (file) {
        std::cout << "\n数据库文件 test.db 已生成！" << std::endl;
        fclose(file);
    }

    return 0;
}
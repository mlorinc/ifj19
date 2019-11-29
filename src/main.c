#include "parser.h"
#include "parser_result.h"
#include "parser_ast.h"

int main(void)
{
    parser_result_t result = parse();

    if (result.ast != NULL) {
        ast_delete(result.ast);
    }

    return result.error_code;
}


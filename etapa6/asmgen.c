#include "asmgen.h"

void generateAsm(TAC *first, char *filename) {
    FILE *fout;
    TAC *tac;

    if(!(fout = fopen(filename,"w"))) {
        fprintf(stderr," ERROR: Cannot open file %s.\n",filename);
        exit(5);
    }

    // Print header
    fprintf(fout, "# START ASM GEN\n"
	                "\t.section	.rodata\n\n");

    // Print hash symbols
    fprintf(fout, "# FOR EACH SYMBOL IN HASH TABLE\n");
    hashPrintAsm(fout);

    fprintf(fout, "\n# STRING\n"
                    ".myString:\n"
                    "\t.string \"%%d\\n\" \n"
                    "\t.text\n\n");

    // Print code
    for(tac = first; tac; tac = tac->next) {
        switch (tac->type) {
            case TAC_BEGINFUN:
                fprintf(fout,   "# BEGIN FUN\n"
                                "\t.globl	%s\n"
                                "\t.type	%s, @function\n"
                                "%s:\n"
                                "\t.cfi_startproc\n"
                                "\tpushq	%%rbp\n"
                                "\tmovq	%%rsp, %%rbp\n\n",
                                tac->res->text,tac->res->text,tac->res->text);
                break;
            case TAC_ENDFUN:
                fprintf(fout, "# END FUN\n"
                                "\tpopq	%%rbp\n"
                                "\tret\n"
                                "\t.cfi_endproc\n\n");
                break;
            case TAC_RETURN:
                fprintf(fout, "# RETURN\n"
                                "\tmovl	_%s(%%rip), %%eax\n\n",
                                tac->res?tac->res->text:"O");
                break;
            case TAC_PRINT:
                fprintf(fout, "# PRINT\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\tmovl	%%eax, %%esi\n"
                                "\tleaq	.meuString(%%rip), %%rdi\n"
                                "\tmovl	$0, %%eax\n"
                                "\tcall	printf@PLT\n\n",
                                tac->res?tac->res->text:"");
                break;
            case TAC_COPY:
                fprintf(fout, "# COPY\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\tmovl	%%eax, _%s(%%rip)\n\n",
                                tac->op1->text, tac->res->text);
                break;
            case TAC_ADD:
                fprintf(fout,"# ADD\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\taddl	%%edx, %%eax\n"
                                "\tmovl	%%eax, _%s(%%rip)\n\n",
                                tac->op1->text,tac->op2->text,tac->res->text);
                break;
            case TAC_SUB:
                fprintf(fout,"# SUB\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\tsubl	%%edx, %%eax\n"
                                "\tmovl	%%eax, _%s(%%rip)\n\n",
                                tac->op1->text,tac->op2->text,tac->res->text);
                break;
            case TAC_LESS:
                fprintf(fout,"# LESS\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
                                "\tmovl	_%s(%%rip), %%eax\n"
                                "\tcmpl	%%edx, %%eax\n"
                                "\tjne ",
                                tac->op1->text,tac->op2->text);
                break;
            case TAC_MULT:
                fprintf(fout,"# MULT\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
	                            "\tmovl	_%s(%%rip), %%eax\n"
	                            "\timul	%%eax, %%edx\n"
                                "\tmovl	%%eax, _%s(%%rip)\n\n",
                                tac->op1->text,tac->op2->text,tac->res->text);
                break;
            case TAC_DIV:
                fprintf(fout,"# DIV\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
	                            "\tmovl	_%s(%%rip), %%eax\n"
	                            "\tidiv	%%eax, %%edx\n"
                                "\tmovl	%%eax, _%s(%%rip)\n\n",
                                tac->op1->text,tac->op2->text,tac->res->text);
                break;
            case TAC_OR:
                fprintf(fout,"# DIV\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
	                            "\tmovl	_%s(%%rip), %%eax\n"
	                            "\tidiv	%%eax, %%edx\n"
                                "\tjne ",
                                tac->op1->text,tac->op2->text);
                break;
            case TAC_EQ:
                fprintf(fout,"# EQUAL\n"
                                "\tmovl	_%s(%%rip), %%edx\n"
	                            "\tmovl	_%s(%%rip), %%eax\n"
	                            "\tcmpl	%%eax, %%edx\n"
                                "\tjne ",
                                tac->op1->text,tac->op2->text);
                break;
            case TAC_IFZ:
                fprintf(fout,"%s\n\n",tac->res->text);
                break;
            case TAC_LABEL:
                fprintf(fout,"# LABEL\n"
                                "%s:\n\n",
                                tac->res->text);
                break;
            case TAC_JUMP:
                fprintf(fout,"# JUMP\n"
                                "\tjmp\t%s\n\n",
                                tac->res->text);
                break;
            case TAC_SYMBOL:
                break;
            default: fprintf(fout,"# TAC NOT IMPLEMENTED: %d\n\n",tac->type);
                break;
        }
    }

    fprintf(fout, "# INFO\n"
                    "\t.size	main, .-main\n"
	                "\t.ident	\"GCC: (Ubuntu 7.2.0-18ubuntu2) 7.2.0\"\n"
	                "\t.section	.note.GNU-stack,\"\",@progbits\n");
}

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	program ();
}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
}

int SyntacticalAnalyzer::program(){
    int errors = 0;

    if(token==LPAREN_T){
        token = lex->GetToken();  // WE APPLIED RULE 1
        errors += define();

        if(token==LPAREN_T){
            token = lex->GetToken();  // consume
            errors+=more_defines();
            if(token==EOF_T)
                token = lex->GetToken();  // consume
            else{
                //cout 
                errors+=1;
            }
        }
        else
            errors+=1;
    }
    else{
        errors +=1;
    }
    return errors;
}


int SyntacticalAnalyzer::stmt(){
	int errors = 0;
	if(token==IDENT_T)
    	token = lex->GetToken();  // consume
	else if (token == LPAREN_T){
    	token = lex->GetToken();  // consume
		errors+= action();
		if(token == RPAREN_T){
    		token = lex->GetToken();  // consume
		}
		else
			errors++;
		}
	else
		errors++;

}

int SyntacticalAnalyzer::stmt_pair_body(){
	int errors = 0;
	if(token == ELSE_T){
    	token = lex->GetToken();  // consume
		errors+=stmt();
		if(token==RPAREN_T)
    		token = lex->GetToken();  // consume
		else
			errors++;
	}
	else{
		errors+=stmt();
		errors+=stmt();
		if(token==RPAREN_T)
    		token = lex->GetToken();  // consume
		else
			errors++;
		errors+=stmt_pair();
	}

	return errors;
}



int SyntacticalAnalyzer::stmt_list()
{
    // [0,"LPAREN_T","EOF_T","IDENT_T","RPAREN_T","DEFINE_T","NUMLIT_T","STRLIT_T","SQUOTE_T","ELSE_T","IF_T","COND_T","LISTOP_T","CONS_T","AND_T","OR_T","NOT_T","NUMBERP_T","LISTP_T","ZEROP_T","NULLP_T","STRINGP_T","PLUS_T","MINUS_T","DIV_T","MULT_T","MODULO_T","ROUND_T","EQUALTO_T","GT_T","LT_T","GTE_T","LTE_T","DISPLAY_T","NEWLINE_T","$"],
    int errors = 0;
    // Rule 5
    if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
    {
        lex->GetToken();
        errors += stmt_list();
    }
    // Rule 6
    if (token == RPAREN_T)
    {
        lex->GetToken();
        errors += stmt_list();
    }
    return errors;
}

int SyntacticalAnalyzer::more_defines(){
	int errors = 0;
    // apply rule 2
    // <more_defines> -> <define> LPARENT_T <more_defines>
    if (token == DEFINE_T)
    {
        // Dont get token because on non terminal
        // lex->GetToken();
        errors += define();
        // dont consume it 
        if (token == LPAREN_T)
        {
            lex->GetToken();
        }
        else
        {
            cout << "ERR LPAREN_T expected" << endl;
        }

        errors += more_defines();
    }
    // apply rules 3
    // <more_defines> -> IDENT_T <stmt_list> RPAREN_T
    if (token == IDENT_T)
    {
        lex->GetToken();
        errors += stmt_list();
        if (token == RPAREN_T)
        {
            // do nothing
        }
        else 
        {
            errors++;
            cout << "Expecting a RPAREN_T" << endl;
        }

    }

    return errors;

}
int SyntacticalAnalyzer::define(){
    int errors = 0;
    if(token == DEFINE_T){
        token = lex->GetToken();  // consume
        if(token==LPAREN_T){
            token = lex->GetToken();  // consume
            if(token==IDENT_T){
                token = lex->GetToken();  // consume
                errors += param_list();
                if(token==RPAREN_T){
                    token = lex->GetToken();  // consume
                    errors+=stmt();
                    errors+=stmt_list();
                    if(token==RPAREN_T){
                        token = lex->GetToken();  // consume
                    }
                    else
                        errors++;
                }
                else
                    errors++;
            }
            else
                errors++;
        }
        else
            errors++;

    }
    else{
        errors++;
    }
    return errors;
}

//action
int SyntacticalAnalyzer::action() {
	int errors = 0;

	switch (token) {
		case IF_T:
			//24
			token = lex->GetToken();
			errors += stmt();
			errors += stmt();
			errors += else_part();
			break;
		case COND_T:
			//25
			token = lex->GetToken();
			if (token == LPAREN_T) {
				token = lex->GetToken();
				errors += stmt_pair_body();
			}
			else {
				errors++;
			}
			break;
		case LISTOP_T:
			//26
			token = lex->GetToken();
			errors += stmt();
			break;
		case CONS_T:
			//27
			token = lex->GetToken();
			errors += stmt();
			errors += stmt();
			break;
		case AND_T:
			//28
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case OR_T:
			//29
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case NOT_T:
			//30
			token = lex->GetToken();
			errors += stmt();
			break;
		case NUMBERP_T:
			//31
			token = lex->GetToken();
			errors += stmt();
			break;
		case LISTP_T:
			//32
			token = lex->GetToken();
			errors += stmt();
			break;
		case ZEROP_T:
			//33
			token = lex->GetToken();
			errors += stmt();
			break;
		case NULLP_T:
			//34
			token = lex->GetToken();
			errors += stmt();
			break;
		case STRINGP_T:
			//35
			token = lex->GetToken();
			errors += stmt();
			break;
		case PLUS_T:
			//36
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case MINUS_T:
			//37
			token = lex->GetToken();
			errors += stmt();
			errors += stmt_list();
			break;
		case DIV_T:
			//38
			token = lex->GetToken();
			errors += stmt();
			errors += stmt_list();
			break;
		case MULT_T:
			//39
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case MODULO_T:
			//40
			token = lex->GetToken();
			errors += stmt();
			errors += stmt();
			break;
		case ROUND_T:
			//41
			token = lex->GetToken();
			errors += stmt();
			break;
		case EQUALTO_T:
			//42
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case GT_T:
			//43
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case LT_T:
			//44
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case GTE_T:
			//45
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case LTE_T:
			//46
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case IDENT_T:
			//47
			token = lex->GetToken();
			errors += stmt_list();
			break;
		case DISPLAY_T:
			//48
			token = lex->GetToken();
			errors += stmt();
			break;
		case NEWLINE_T:
			//49
			token = lex->GetToken();
			break;
		default:
			errors++;
			break;
	}

	return errors;
}

//any_other_token
int SyntacticalAnalyzer::any_other_token() {
	int errors = 0;

	switch (token) {
		case LPAREN_T:
			//50
			token = lex->GetToken();
			errors += more_tokens();
			if (token == RPAREN_T) {
				token = lex->GetToken();
			}
			else {
				errors++;
			}
			break;
		case IDENT_T:
			//51
			token = lex->GetToken();
			break;
		case NUMLIT_T:
			//52
			token = lex->GetToken();
			break;
		case STRLIT_T:
			//53
			token = lex->GetToken();
			break;
		case CONS_T:
			//54
			token = lex->GetToken();
			break;
		case IF_T:
			//55
			token = lex->GetToken();
			break;
		case DISPLAY_T:
			//56
			token = lex->GetToken();
			break;
		case NEWLINE_T:
			//57
			token = lex->GetToken();
			break;
		case LISTOP_T:
			//58
			token = lex->GetToken();
			break;
		case AND_T:
			//59
			token = lex->GetToken();
			break;
		case OR_T:
			//60
			token = lex->GetToken();
			break;
		case NOT_T:
			//61
			token = lex->GetToken();
			break;
		case DEFINE_T:
			//62
			token = lex->GetToken();
			break;
		case NUMBERP_T:
			//63
			token = lex->GetToken();
			break;
		case LISTP_T:
			//64
			token = lex->GetToken();
			break;
		case ZEROP_T:
			//65
			token = lex->GetToken();
			break;
		case NULLP_T:
			//66
			token = lex->GetToken();
			break;
		case STRINGP_T:
			//67
			token = lex->GetToken();
			break;
		case PLUS_T:
			//68
			token = lex->GetToken();
			break;
		case MINUS_T:
			//69
			token = lex->GetToken();
			break;
		case DIV_T:
			//70
			token = lex->GetToken();
			break;
		case MULT_T:
			//71
			token = lex->GetToken();
			break;
		case MODULO_T:
			//72
			token = lex->GetToken();
			break;
		case ROUND_T:
			//73
			token = lex->GetToken();
			break;
		case EQUALTO_T:
			//74
			token = lex->GetToken();
			break;
		case GT_T:
			//75
			token = lex->GetToken();
			break;
		case LT_T:
			//76
			token = lex->GetToken();
			break;
		case GTE_T:
			//77
			token = lex->GetToken();
			break;
		case LTE_T:
			//78
			token = lex->GetToken();
			break;
		case SQUOTE_T:
			//79
			token = lex->GetToken();
			errors += any_other_token();
			break;
		case COND_T:
			//80
			token = lex->GetToken();
			break;
		case ELSE_T:
			//81
			token = lex->GetToken();
			break;
		default:
			//Error
			errors++;
			break;
	}

	return errors;
}

//stmt_pair
int SyntacticalAnalyzer::stmt_pair() {
	int errors = 0;

	//If LPAREN_T, apply rule 20.
	if (token == LPAREN_T) {
		token = lex->GetToken();
		errors += stmt_pair_body();
	}
	//Else, apply rule 21 (lambda).

	return errors;
}

//param_list
int SyntacticalAnalyzer::param_list() {
	int errors = 0;

	//If IDENT_T, apply rule 16. 
	if (token == IDENT_T) {
		token = lex->GetToken(); 
		errors += param_list();
	}
	//Else, apply rule 17 (lambda).

	return errors;
}

int SyntacticalAnalyzer::else_part()
{
    // [0,"LPAREN_T","EOF_T","IDENT_T","RPAREN_T","DEFINE_T","NUMLIT_T","STRLIT_T","SQUOTE_T","ELSE_T","IF_T","COND_T","LISTOP_T","CONS_T","AND_T","OR_T","NOT_T","NUMBERP_T","LISTP_T","ZEROP_T","NULLP_T","STRINGP_T","PLUS_T","MINUS_T","DIV_T","MULT_T","MODULO_T","ROUND_T","EQUALTO_T","GT_T","LT_T","GTE_T","LTE_T","DISPLAY_T","NEWLINE_T","$"],
    int errors = 0;
    // Rule 18
    if (token == LPAREN_T || token == IDENT_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
    {
        lex->GetToken();
        errors += else_part();
    }
    // Rule 19
    else if (token == RPAREN_T)
    {
        lex->GetToken();
        errors += else_part();
    }
    return errors;
}
int SyntacticalAnalyzer::quoted_lit()
{
    int errors = 0;
    if (token == EOF_T || token == RPAREN_T) {
        errors++;
        cout << "ERR on quoted_lit..." << endl;
    }
    else
    {
        lex->GetToken();
        errors += quoted_lit();
    }
    return errors;
}
int SyntacticalAnalyzer::literal()
{
    int errors = 0;
    if (token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T)
    {
        lex->GetToken();
        errors += literal();
    }
    return errors;
}


int SyntacticalAnalyzer::more_tokens()
{
        

    	// [0,14,83,14,15,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,83], // <more_tokens>
        
    int errors = 0;
    // Rule 14
    if (token == EOF_T) 
    {
        errors++;
    }
    // Rule 15
    if (token == RPAREN_T)
    {
        errors += more_tokens();
    }
    // 5 - 34
    // token == DEFINE_T || token == NUMLIT_T || token == STRLIT_T || token == SQUOTE_T || token == ELSE_T || token == IF_T || token == COND_T || token == LISTOP_T || token == CONS_T || token == AND_T || token == OR_T || token == NOT_T || token == NUMBERP_T || token == LISTP_T || token == ZEROP_T || token == NULLP_T || STRINGP_T,PLUS_T,MINUS_T","DIV_T","MULT_T","MODULO_T || ROUND_T || EQUALTO_T || GT_T || LT_T || GTE_T || LTE_T || DISPLAY_T || NEWLINE_T
    return errors;
}


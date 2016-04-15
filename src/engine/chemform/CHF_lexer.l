%%

"."[0-9]*"H2O"			return(CHF_HYDRATE);
[A-Z]{1}[a-z]{0,1}      return(CHF_NAME);
[0-9]+	                return(CHF_NUM);
"("	                return(CHF_LPAREN);
")"	                return(CHF_RPAREN);
\n                      return(CHF_EOL);

%%

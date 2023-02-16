


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <dirent.h>
#include <sys/mman.h>
#include <sys/stat.h>
//-----WINDOWS-----
//#include <windows.h>

//----linux-----
#include <inttypes.h>
#include <libgen.h>
#include <ctype.h>

#define KCE_MODE_INPUT 0
#define KCE_MODE_FILES 1
#define KCE_MODE_FILES_INPUT 2

#define KCE_RES "\x1B[0m"
#define KCE_BLACK "\x1B[30m"
#define KCE_RED "\x1B[31m"
#define KCE_GREEN "\x1B[32m"
#define KCE_YELLOW "\x1B[33m"
#define KCE_BLUE "\x1B[34m"
#define KCE_MAGENTA "\x1B[35m"
#define KCE_CYAN "\x1B[36m"
#define KCE_WHITE "\x1B[37m"

#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

#define MAX_SUGGESTIONS_LIST_SIZE 256
#define MAX_SUGGESTION_NAME_LENGTH 64

#define BRACKETS_COLORS 4

#define KCE_ERROR(x) printf("%s%s%s\n", KCE_RED, x, KCE_RES);
/*** defines ***/

#define KCE_VERSION "0.1"
#define KCE_TAB_STOP 8
#define KCE_QUIT_TIMES 3

#define MAX_COLOR_CHARS 16

// typedef struct {
//     int line;
//     int column;
// } Bracket;

// typedef struct {
//     Bracket open;
//     Bracket close;
// } BracketPair;

// BracketPair brackets_squared[1024];
// BracketPair brackets_rounded[1024];
// BracketPair brackets_shaped[1024];

// int brackets_squared_count = 0;
// int brackets_rounded_count = 0;
// int brackets_shaped_count = 0;

#define CTRL_KEY(k) ((k) & 0x1f)

char mlcommentCol[MAX_COLOR_CHARS] = "\x1B[36m";
char keyword1Col[MAX_COLOR_CHARS] = "\x1B[31m";
char keyword2Col[MAX_COLOR_CHARS] = "\x1B[32m";
char stringCol[MAX_COLOR_CHARS] = "\x1B[35m";
char numberCol[MAX_COLOR_CHARS] = "\x1B[34m";
char matchCol[MAX_COLOR_CHARS] = "\x1B[34m";
char defaultCol[MAX_COLOR_CHARS] = "\x1B[37m";
char dopCol[MAX_COLOR_CHARS] = "\x1B[33m";
char errorBracketsCol[MAX_COLOR_CHARS] = "\x1B[31m";

char barcketsColors[BRACKETS_COLORS][MAX_COLOR_CHARS] = {KCE_YELLOW, KCE_MAGENTA, KCE_WHITE, KCE_CYAN};
int openedBracketsSquare = 0;
int openedBracketsShaped = 0;
int openedBracketsRounded = 0;

char lineNumberCol[MAX_COLOR_CHARS] = "\x1B[37m";
char currentRowNumberCol[MAX_COLOR_CHARS] = "\x1B[33m";

char pythonLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char cLogoCol[MAX_COLOR_CHARS] = "\x1B[34m";
char hLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char RLogoCol[MAX_COLOR_CHARS] = "\x1B[31m";
char objCLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char asmLogoCol[MAX_COLOR_CHARS] = "\x1B[31m";
char rubyLogoCol[MAX_COLOR_CHARS] = "\x1B[31m";
char rustLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char shellLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char cppLogoCol[MAX_COLOR_CHARS] = "\x1B[34m";
char hppLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char jsLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char tsLogoCol[MAX_COLOR_CHARS] = "\x1B[34m";
char csLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char fsLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char kotlinLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char goLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char perlLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char juliaLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char vbLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char lispLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char javaLogoCol[MAX_COLOR_CHARS] = "\x1B[31m";
char phpLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char sqlLogoCol[MAX_COLOR_CHARS] = "\x1B[34m";
char htmlLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char cssLogoCol[MAX_COLOR_CHARS] = "\x1B[34m";
char luaLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char tclLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char exeLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char batLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char jsonLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char dartLogoCol[MAX_COLOR_CHARS] = "\x1B[32m";
char pascalLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char dLogoCol[MAX_COLOR_CHARS] = "\x1B[31m";
char fortranLogoCol[MAX_COLOR_CHARS] = "\x1B[35m";
char kLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";
char lLogoCol[MAX_COLOR_CHARS] = "\x1B[33m";

char dirLogoCol[MAX_COLOR_CHARS] = "\x1B[36m";
char dirEndCol[MAX_COLOR_CHARS] = "\x1B[35m";

char selectionCol[MAX_COLOR_CHARS] = "\x1b[42m";
char backgroundCol[MAX_COLOR_CHARS] = "\x1b[0m";

int numrowscharcount=0;
int offsetf=0; 

typedef struct _selectionLine{
  int line1;
  int line2;
  int from;
  int to;
} selectionLine;
selectionLine selection;

#define MAX_LINE_LEN 64
char dirString[MAX_LINE_LEN];
#pragma region KCE_LOADER




#pragma endregion
uint_fast8_t mode = KCE_MODE_INPUT;

bool drawLineNum = true;

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN,
  INSERT
};

enum editorHighlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH,
  HL_DOP,
  HL_BRACKETS_1,
  HL_BRACKETS_2,
  HL_BRACKETS_3,
  HL_BRACKETS_4,
  HL_BRACKETS_ERROR
};


#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

/*** data ***/

struct editorSyntax {
  char *filetype;
  char **filematch;
  char **keywords;
  char **secondKeywords;
  char *singleline_comment_start;
  char *multiline_comment_start;
  char *multiline_comment_end;
  int flags;
  char *dop1;
  char *dop2;
};

typedef struct erow {
  int idx;
  int size;
  int rsize;
  char *chars;
  char *render;
  unsigned char *hl;
  int hl_open_comment;
} erow;

struct editorConfig {
  int cx, cy;
  int rx;
  int rowoff;
  int coloff;
  int screenrows;
  int screencols;
  int numrows;
  erow *row;
  int dirty;
  char *filename;
  char statusmsg[80];
  time_t statusmsg_time;
  struct editorSyntax *syntax;
  struct termios orig_termios;
};

struct editorConfig E;

/*** filetypes ***/

char *C_HL_extensions[] = { ".c", ".h", NULL };

char *C_HL_keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "static|", "enum", "case",
  "volatile", "const|", "register", "signed|", "unsigned|", "short", "long",
  "int", "char", "float", "double", "void", "goto", "default", "NULL|", NULL
};
char *C_HL_secondKeywords[] = {
	"", NULL
};
//C++
char *Cpp_HL_extensions[] = { ".cpp", ".cc", ".cxx", ".hpp", ".hh", ".hxx", NULL };

char *Cpp_HL_keywords[] = {
  "switch", "if", "while", "for", "break", "continue", "return", "else",
  "struct", "union", "typedef", "static", "enum", "class", "case",
  "volatile", "const", "register", "signed", "unsigned", "short", "long",
  "int", "char", "float", "double", "void", "goto", "default",

  "inline|", "auto|", "extern|", "register|", "sizeof|", "typedef|",
  "volatile|", "const|", "static|", "signed|", "unsigned|", 

  "namespace|", "template|", "using|", "throw|", "try|", "catch|",
  "new|", "delete|", "public|", "protected|", "private|", "friend|",
  "explicit|", "this|", "virtual|", "override|", "final|", "mutable|",
  "constexpr|", "noexcept|", "nullptr|", "true|", "false|", NULL
};
//Python
char *PY_HL_extensions[] = {".py", ".pyw", NULL};
char *PY_HL_keywords[] = {
  "and", "as", "assert", "break", "class", "continue", "def", "del", "elif",
  "else", "except", "False|", "finally", "for", "from", "global", "if", "import",
  "in", "is", "lambda", "None|", "not", "or", "pass", "raise", "return",
  "True|", "try", "while", "with", "yield", "async", NULL
};
// Ruby
char *RB_HL_extensions[] = {".rb", ".erb", ".ru", ".rake", NULL};
char *RB_HL_keywords[] = {
  "alias", "and", "begin", "break", "case", "class", "def", "defined?",
  "do", "else", "elsif", "end", "ensure", "false", "for", "if", "in",
  "module", "next", "nil", "not", "or", "redo", "rescue", "retry",
  "return", "self", "super", "then", "true|", "undef", "unless", "until",
  "when", "while", "yield", NULL
};

// Rust
char *RS_HL_extensions[] = {".rs", NULL};
char *RS_HL_keywords[] = {
  "as", "break", "const", "continue", "crate", "else", "enum", "extern",
  "false", "fn", "for", "if", "impl", "in", "let", "loop", "match", "mod",
  "move", "mut", "pub", "ref", "return", "self", "static", "struct", "super",
  "trait", "true", "type", "unsafe", "use", "where", "while", NULL
};

// JavaScript
char *JS_HL_extensions[] = {".js", ".mjs", ".jsx", NULL};
char *JS_HL_keywords[] = {
  "async", "await", "break", "case", "catch", "class", "const", "continue",
  "debugger", "default", "delete", "do", "else", "export", "extends", "false",
  "finally", "for", "function", "if", "import", "in", "instanceof", "new",
  "null", "return", "super", "switch", "this", "throw", "true", "try",
  "typeof", "var", "void", "while", "with", "yield", NULL
};
//Java
char *JAVA_HL_extensions[] = { ".java", NULL };
char *JAVA_HL_keywords[] = {
  "abstract", "continue", "for", "new", "switch",
  "assert", "default", "goto", "package", "synchronized",
  "boolean", "do", "if", "private", "this",
  "break", "double", "implements", "protected", "throw",
  "byte", "else", "import", "public", "throws",
  "case", "enum", "instanceof", "return", "transient",
  "catch", "extends", "int", "short", "try",
  "char", "final", "interface", "static", "void",
  "class", "finally", "long", "strictfp", "volatile",
  "const", "float", "native", "super", "while",
  NULL
};
//Json
char *JSON_HL_extensions[] = { ".json", NULL };
char *JSON_HL_keywords[] = {
  "false", "null", "true",
  NULL
};
//TypeScript
char *TS_HL_extensions[] = { ".ts", NULL };
char *TS_HL_keywords[] = {
  "as", "break", "case", "catch", "class", "const",
  "continue", "debugger", "default", "delete", "do",
  "else", "enum", "export", "extends", "false",
  "finally", "for", "function", "if", "import",
  "in", "instanceof", "new", "null", "return",
  "super", "switch", "this", "throw", "true",
  "try", "typeof", "var", "void", "while",
  "with", "let", "async", "await",
  NULL
};
//SHELL
char *SH_HL_extensions[] = { ".sh", ".bash", NULL };

char *SH_HL_keywords[] = {
"if", "then", "else", "elif", "fi", "while", "do", "done", "for", "in", "case", "esac",
"function", "return", "local", "echo", "cd", "pwd", "export", "unset", "test", "shift", "exec",
"true", "false", "readonly", "break", "continue", "eval", "wait", "trap", "kill", "alias", "unalias",
"set", "unset", "env", "source", "bg", "fg", "jobs", "history", "fc", "export", "declare", "typeset", NULL
};
//KCEConf
char *KConf_HL_extensions[] = { ".kceconf", NULL };
char *KConf_HL_keywords[] = {
  "[", "]",
  NULL
};

struct editorSyntax HLDB[] = {
  {
    "c",
    C_HL_extensions,
    C_HL_keywords,
    C_HL_secondKeywords,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "cpp",
    Cpp_HL_extensions,
    Cpp_HL_keywords,
    NULL,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "python",
    PY_HL_extensions,
    PY_HL_keywords,
    NULL,
    "#", "\"\"", "\"\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "@"
  },
  {
    "ruby",
    RB_HL_extensions,
    RB_HL_keywords,
    NULL,
    "#", "=begin", "=end",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "rust",
    RS_HL_extensions,
    RS_HL_keywords,
    NULL,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "javascript",
    JS_HL_extensions,
    JS_HL_keywords,
    NULL,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "java",
    JAVA_HL_extensions,
    JAVA_HL_keywords,
    NULL,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "json",
    JSON_HL_extensions,
    JSON_HL_keywords,
    NULL,
    "\"_comment\"", "\"__comment\"", "\"__comment\"",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "typescript",
    TS_HL_extensions,
    TS_HL_keywords,
    NULL,
    "//", "/*", "*/",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "kceconf",
    KConf_HL_extensions,
    KConf_HL_keywords,
    NULL,
    ";", ";", ";",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  },
  {
    "shell",
    SH_HL_extensions,
    SH_HL_keywords,
    NULL,
    "#", ": \'", "\'",
    HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS,
    "#",
    "#"
  }
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))


char suggestions[MAX_SUGGESTIONS_LIST_SIZE][MAX_SUGGESTION_NAME_LENGTH];
int suggestionsCount = 0;

/*** prototypes ***/

void editorSetStatusMessage(const char *fmt, ...);
void editorRefreshScreen();
char *editorPrompt(char *prompt, void (*callback)(char *, int));

/*** terminal ***/

void die(const char *s) {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = E.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

int editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }

  if (c == '\x1b') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }

    return '\x1b';
  } else {
    return c;
  }
}

int getCursorPosition(int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;

  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;

  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';

  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;

  return 0;
}

int getWindowSize(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
    return getCursorPosition(rows, cols);
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

/* suggestions */
bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
void autocomplete(const char *prefix)
{
    int i;
    for (i = 0; i < suggestionsCount; i++)
    {
        if (startsWith(prefix, suggestions[i]))
        {
            printf("%s\n", suggestions[i]);
        }
    }
}


/*** syntax highlighting ***/

int is_separator(int c) {
  return isspace(c) || c == '\0' || strchr(",.()+-/*=~%%<>[];", c) != NULL;
}

void editorUpdateSyntax(erow *row) {
  row->hl = realloc(row->hl, row->rsize);
  memset(row->hl, HL_NORMAL, row->rsize);

  if (E.syntax == NULL) return;

  char **keywords = E.syntax->keywords;
  char **secondKeywords = E.syntax->secondKeywords;
  char *scs = E.syntax->singleline_comment_start;
  char *dps = E.syntax->dop1;
  char *dpe = E.syntax->dop2;
  char *mcs = E.syntax->multiline_comment_start;
  char *mce = E.syntax->multiline_comment_end;

  int scs_len = scs ? strlen(scs) : 0;
  int dps_len = dps ? strlen(dps) : 0;
  int dpe_len = dps ? strlen(dpe) : 0;
  int mcs_len = mcs ? strlen(mcs) : 0;
  int mce_len = mce ? strlen(mce) : 0;

  int prev_sep = 1;
  int in_string = 0;
  int in_comment = (row->idx > 0 && E.row[row->idx - 1].hl_open_comment);

  int i = 0;
  while (i < row->rsize) {
    char c = row->render[i];
    unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;
    if (scs_len && !in_string && !in_comment) {
      if (!strncmp(&row->render[i], scs, scs_len)) {
        memset(&row->hl[i], HL_COMMENT, row->rsize - i);
        break;
      }
    }
    if (dps_len && !in_string && !in_comment) {
      if (!strncmp(&row->render[i], dps, dps_len)) {
        memset(&row->hl[i], HL_DOP, row->rsize - i);
        break;
      }
    }
    if (dpe_len && !in_string && !in_comment) {
      if (!strncmp(&row->render[i], dpe, dpe_len)) {
        memset(&row->hl[i], HL_DOP, row->rsize - i);
        break;
      }
    }
    if (mcs_len && mce_len && !in_string) {
      if (in_comment) {
        row->hl[i] = HL_MLCOMMENT;
        if (!strncmp(&row->render[i], mce, mce_len)) {
          memset(&row->hl[i], HL_MLCOMMENT, mce_len);
          i += mce_len;
          in_comment = 0;
          prev_sep = 1;
          continue;
        } else {
          i++;
          continue;
        }
      } else if (!strncmp(&row->render[i], mcs, mcs_len)) {
        memset(&row->hl[i], HL_MLCOMMENT, mcs_len);
        i += mcs_len;
        in_comment = 1;
        continue;
      }
    }

    if (E.syntax->flags & HL_HIGHLIGHT_STRINGS) {
      if (in_string) {
        row->hl[i] = HL_STRING;
        if (c == '\\' && i + 1 < row->rsize) {
          row->hl[i + 1] = HL_STRING;
          i += 2;
          continue;
        }
        if (c == in_string) in_string = 0;
        i++;
        prev_sep = 1;
        continue;
      } else {
        if (c == '"' || c == '\'') {
          in_string = c;
          row->hl[i] = HL_STRING;
          i++;
          continue;
        }
      }
    }
    //----------brackets-----------
    // if(row->render[i] == '{' && !in_string && !in_comment){
      
    // }
    if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
      if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) ||
          (c == '.' && prev_hl == HL_NUMBER)) {
        row->hl[i] = HL_NUMBER;
        i++;
        prev_sep = 0;
        continue;
      }
    }
    if (prev_sep) {
      int j;
      for (j = 0; keywords[j]; j++) {
        int klen = strlen(keywords[j]);
        int kw2 = keywords[j][klen - 1] == '|';
        if (kw2) klen--;

        if (!strncmp(&row->render[i], keywords[j], klen) &&
            is_separator(row->render[i + klen])) {
          memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, klen);
          i += klen;
          break;
        }
      }
      if (keywords[j] != NULL) {
        prev_sep = 0;
        continue;
      }
    }

    prev_sep = is_separator(c);
    i++;
  }
  int changed = (row->hl_open_comment != in_comment);
  row->hl_open_comment = in_comment;
  if (changed && row->idx + 1 < E.numrows)
    editorUpdateSyntax(&E.row[row->idx + 1]);
}

char* editorSyntaxToColor(int hl) {
  switch (hl) {
    case HL_COMMENT:
    case HL_MLCOMMENT: return mlcommentCol;
    case HL_KEYWORD1: return keyword1Col;
    case HL_KEYWORD2: return keyword2Col;
    case HL_STRING: return stringCol;
    case HL_NUMBER: return numberCol;
    case HL_MATCH: return matchCol;
    case HL_DOP: return dopCol;
    case HL_BRACKETS_1: return barcketsColors[0];
    case HL_BRACKETS_2: return barcketsColors[1];
    case HL_BRACKETS_3: return barcketsColors[2];
    case HL_BRACKETS_4: return barcketsColors[3];
    case HL_BRACKETS_ERROR: return errorBracketsCol;
    default: return defaultCol;
  }
}

void editorSelectSyntaxHighlight() {
  E.syntax = NULL;
  if (E.filename == NULL) return;

  for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
    struct editorSyntax *s = &HLDB[j];
    unsigned int i = 0;
    while (s->filematch[i]) {
      char *p = strstr(E.filename, s->filematch[i]);
      if (p != NULL) {
        int patlen = strlen(s->filematch[i]);
        if (s->filematch[i][0] != '.' || p[patlen] == '\0') {
          E.syntax = s;

          int filerow;
          for (filerow = 0; filerow < E.numrows; filerow++) {
            editorUpdateSyntax(&E.row[filerow]);
          }

          return;
        }
      }
      i++;
    }
  }
}

/*** row operations ***/

int editorRowCxToRx(erow *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (KCE_TAB_STOP - 1) - (rx % KCE_TAB_STOP);
    rx++;
  }
  return rx;
}

int editorRowRxToCx(erow *row, int rx) {
  int cur_rx = 0;
  int cx;
  for (cx = 0; cx < row->size; cx++) {
    if (row->chars[cx] == '\t')
      cur_rx += (KCE_TAB_STOP - 1) - (cur_rx % KCE_TAB_STOP);
    cur_rx++;

    if (cur_rx > rx) return cx;
  }
  return cx;
}

void editorUpdateRow(erow *row) {
  int tabs = 0;
  int j;
  for (j = 0; j < row->size; j++)
    if (row->chars[j] == '\t') tabs++;

  free(row->render);
  row->render = malloc(row->size + tabs*(KCE_TAB_STOP - 1) + 1);

  int idx = 0;
  for (j = 0; j < row->size; j++) {
    if (row->chars[j] == '\t') {
      row->render[idx++] = ' ';
      while (idx % KCE_TAB_STOP != 0) row->render[idx++] = ' ';
    } else {
      row->render[idx++] = row->chars[j];
    }
  }
  row->render[idx] = '\0';
  row->rsize = idx;
  editorUpdateSyntax(row);
}

void editorInsertRow(int at, char *s, size_t len) {
  if (at < 0 || at > E.numrows) return;

  E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
  memmove(&E.row[at + 1], &E.row[at], sizeof(erow) * (E.numrows - at));
  for (int j = at + 1; j <= E.numrows; j++) E.row[j].idx++;

  E.row[at].idx = at;

  E.row[at].size = len;
  E.row[at].chars = malloc(len + 1);
  memcpy(E.row[at].chars, s, len);
  E.row[at].chars[len] = '\0';

  E.row[at].rsize = 0;
  E.row[at].render = NULL;
  E.row[at].hl = NULL;
  E.row[at].hl_open_comment = 0;
  editorUpdateRow(&E.row[at]);

  E.numrows++;
  E.dirty++;
}

void editorFreeRow(erow *row) {
  free(row->render);
  free(row->chars);
  free(row->hl);
}

void editorDelRow(int at) {
  if (at < 0 || at >= E.numrows) return;
  editorFreeRow(&E.row[at]);
  memmove(&E.row[at], &E.row[at + 1], sizeof(erow) * (E.numrows - at - 1));
  for (int j = at; j < E.numrows - 1; j++) E.row[j].idx--;
  E.numrows--;
  E.dirty++;
}

void editorRowInsertChar(erow *row, int at, int c) {
  if (at < 0 || at > row->size) at = row->size;
  row->chars = realloc(row->chars, row->size + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->size - at + 1);
  row->size++;
  row->chars[at] = c;
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowAppendString(erow *row, char *s, size_t len) {
  row->chars = realloc(row->chars, row->size + len + 1);
  memcpy(&row->chars[row->size], s, len);
  row->size += len;
  row->chars[row->size] = '\0';
  editorUpdateRow(row);
  E.dirty++;
}

void editorRowDelChar(erow *row, int at) {
  if (at < 0 || at >= row->size) return;
  memmove(&row->chars[at], &row->chars[at + 1], row->size - at);
  row->size--;
  editorUpdateRow(row);
  E.dirty++;
}

/*** editor operations ***/

void editorInsertChar(int c) {
  if (E.cy == E.numrows) {
    editorInsertRow(E.numrows, "", 0);
  }
  editorRowInsertChar(&E.row[E.cy], E.cx, c);
  E.cx++;
}

void editorInsertNewline() {
  if (E.cx == 0) {
    editorInsertRow(E.cy, "", 0);
  } else {
    erow *row = &E.row[E.cy];
    editorInsertRow(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
    row = &E.row[E.cy];
    row->size = E.cx;
    row->chars[row->size] = '\0';
    editorUpdateRow(row);
  }
  E.cy++;
  E.cx = 0;
}

void editorDelChar() {
  if (E.cy == E.numrows) return;
  if (E.cx == 0 && E.cy == 0) return;

  erow *row = &E.row[E.cy];
  if (E.cx > 0) {
    editorRowDelChar(row, E.cx - 1);
    E.cx--;
  } else {
    E.cx = E.row[E.cy - 1].size;
    editorRowAppendString(&E.row[E.cy - 1], row->chars, row->size);
    editorDelRow(E.cy);
    E.cy--;
  }
}

/*** file i/o ***/

char *editorRowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < E.numrows; j++)
    totlen += E.row[j].size + 1;
  *buflen = totlen;

  char *buf = malloc(totlen);
  char *p = buf;
  for (j = 0; j < E.numrows; j++) {
    memcpy(p, E.row[j].chars, E.row[j].size);
    p += E.row[j].size;
    *p = '\n';
    p++;
  }

  return buf;
}

void editorOpen(char *filename) {
  free(E.filename);
  E.filename = strdup(filename);
  editorSelectSyntaxHighlight();

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    fp = fopen(filename, "w+");
    //die("fopen");
  }
  char *path = NULL;
  char buf[1024];
  if(fgets(buf, sizeof(buf), fp) != NULL){
	path = dirname(buf);
  }
  strcpy(dirString, path);
 
 // realpath(filename, dirString);
  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;
  while ((linelen = getline(&line, &linecap, fp)) != -1) {
    while (linelen > 0 && (line[linelen - 1] == '\n' ||
                           line[linelen - 1] == '\r'))
      linelen--;
    editorInsertRow(E.numrows, line, linelen);
  }
  free(line);
  fclose(fp);
  E.dirty = 0;
}

void editorSave() {
  if (E.filename == NULL) {
    E.filename = editorPrompt("Save as: %s (ESC to cancel)", NULL);
    if (E.filename == NULL) {
      editorSetStatusMessage("Save aborted");
      return;
    }
    editorSelectSyntaxHighlight();
  }

  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        E.dirty = 0;
        editorSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}
void editorSaveAs(char *filename) {
  int len;
  char *buf = editorRowsToString(&len);

  int fd = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        editorSetStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }

  free(buf);
  editorSetStatusMessage("Can't save! I/O error: %s", strerror(errno));
}

/*** find ***/

void editorFindCallback(char *query, int key) {
  static int last_match = -1;
  static int direction = 1;

  static int saved_hl_line;
  static char *saved_hl = NULL;

  if (saved_hl) {
    memcpy(E.row[saved_hl_line].hl, saved_hl, E.row[saved_hl_line].rsize);
    free(saved_hl);
    saved_hl = NULL;
  }

  if (key == '\r' || key == '\x1b') {
    last_match = -1;
    direction = 1;
    return;
  } else if (key == ARROW_RIGHT || key == ARROW_DOWN) {
    direction = 1;
  } else if (key == ARROW_LEFT || key == ARROW_UP) {
    direction = -1;
  } else {
    last_match = -1;
    direction = 1;
  }

  if (last_match == -1) direction = 1;
  int current = last_match;
  int i;
  for (i = 0; i < E.numrows; i++) {
    current += direction;
    if (current == -1) current = E.numrows - 1;
    else if (current == E.numrows) current = 0;

    erow *row = &E.row[current];
    char *match = strstr(row->render, query);
    if (match) {
      last_match = current;
      E.cy = current;
      E.cx = editorRowRxToCx(row, match - row->render);
      E.rowoff = E.numrows;

      saved_hl_line = current;
      saved_hl = malloc(row->rsize);
      memcpy(saved_hl, row->hl, row->rsize);
      memset(&row->hl[match - row->render], HL_MATCH, strlen(query));
      break;
    }
  }
}

void editorFind() {
  int saved_cx = E.cx;
  int saved_cy = E.cy;
  int saved_coloff = E.coloff;
  int saved_rowoff = E.rowoff;

  char *query = editorPrompt("Search: %s (Use ESC/Arrows/Enter)",
                             editorFindCallback);

  if (query) {
    free(query);
  } else {
    E.cx = saved_cx;
    E.cy = saved_cy;
    E.coloff = saved_coloff;
    E.rowoff = saved_rowoff;
  }
}
void editorMoveCursorToLine(int line) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  E.cy = (line-1);

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}
void editorLineMode();
void initEditor();
void editorCommandCallback(char *query, int key) {
  if (key == CTRL_KEY('v')) {
    //mode = KCE_MODE_INPUT;
    return;
  }
  if(key == '\r' || key == '\x1b' || key == ARROW_UP || key == ARROW_DOWN || key == '\n'){
    //:q - save and exit
    if(strcmp(query, ":q") == 0) {
      editorSave();
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
    }
    //:x - exit without saving
    if(strcmp(query, ":x") == 0) {
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
    }
    //:lm - enter line mode
    if(strcmp(query, ":lm") == 0) {
      editorLineMode();
      return;
    }
    //:l - without line mode move cursor to line
    if(query[0] == ':' && query[1] == 'l' && query[2] == ' ') {
      char bufG[24] = "";
      for(int i = 3; i < strlen(query); i++) {
        strncat(bufG, &query[i], 1);
      }
      editorMoveCursorToLine(atoi(bufG));
    }
    //:s - save file
    if(strcmp(query, ":s") == 0) {
      editorSave();
    }
    //:sav <filename> - save file as
    if(query[0] == ':' && query[1] == 's' && query[2] == 'a' && query[3] == 'v' && query[4] == ' ') {
      char bufG[256] = "";
      for(int i = 5; i < strlen(query); i++) {
        strncat(bufG, &query[i], 1);
      }
      editorSaveAs(bufG);
    }

    //:e <filename> - open file
    if(query[0] == ':' && query[1] == 'e' && query[2] == ' ') {
      char bufG[256] = "";
      for(int i = 3; i < strlen(query); i++) {
        strncat(bufG, &query[i], 1);
      }
      enableRawMode();
      initEditor();
      editorOpen(bufG);
    }
    return;
  }

  
}
void editorLineCallback(char *query, int key) {
  if (key == CTRL_KEY('l')) {
    //mode = KCE_MODE_INPUT;
    return;
  } 
  else if(key == ARROW_UP || key == ARROW_DOWN || key == '\n' || key == '\r' || key == '\x1b'){
    editorMoveCursorToLine(atoi(query));
    return;
  }

  
}


void editorCommandMode() {
  int saved_cx = E.cx;
  int saved_cy = E.cy;
  int saved_coloff = E.coloff;
  int saved_rowoff = E.rowoff;
  char *query = editorPrompt("Command: %s",
                             editorCommandCallback);

  if (query) {
    free(query);
  } else {
    E.cx = saved_cx;
    E.cy = saved_cy;
    E.coloff = saved_coloff;
    E.rowoff = saved_rowoff;
  }
}

void editorLineMode() {
  int saved_cx = E.cx;
  int saved_cy = E.cy;
  int saved_coloff = E.coloff;
  int saved_rowoff = E.rowoff;
  char *query = editorPrompt("Line: %s",
                             editorLineCallback);

  if (query) {
    free(query);
  } else {
    E.cx = saved_cx;
    E.cy = saved_cy;
    E.coloff = saved_coloff;
    E.rowoff = saved_rowoff;
  }
}

/*** append buffer ***/

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

void abAppend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}

void abFree(struct abuf *ab) {
  free(ab->b);
}

/*** output ***/

void editorScroll() {
  E.rx = 0;
  if (E.cy < E.numrows) {
    E.rx = editorRowCxToRx(&E.row[E.cy], E.cx);
  }

  if (E.cy < E.rowoff) {
    E.rowoff = E.cy;
  }
  if (E.cy >= E.rowoff + E.screenrows) {
    E.rowoff = E.cy - E.screenrows + 1;
  }
  if (E.rx < E.coloff) {
    E.coloff = E.rx;
  }
  if (E.rx >= E.coloff + E.screencols) {
    E.coloff = E.rx - E.screencols + 1;
  }
}

void editorDrawRows(struct abuf *ab) {
  int y;
  DIR *dir;
  struct dirent *ent;
  struct stat statbuf;
  int longestDir = 20;
  dir = opendir(dirString);
  for (y = 0; y < E.screenrows; y++) {
    int filerow = y + E.rowoff;
    if(mode == KCE_MODE_FILES || mode == KCE_MODE_FILES_INPUT){
      offsetf = 26;
      if(y == 0){
        abAppend(ab, "------------------------", 24);
      }
      else{
        if (dir != NULL) {
          if((ent = readdir (dir)) != NULL){
            //[▄
            // if(strlen(ent->d_name) > longestDir){
            //   longestDir = strlen(ent->d_name);
            // }
            stat(ent->d_name, &statbuf);
            if (S_ISDIR(statbuf.st_mode)){
              abAppend(ab, " ", 1);
              char str23[MAX_COLOR_CHARS*2+2] = "";
              sprintf(str23, "%s►%s", dirLogoCol, defaultCol);
              abAppend(ab, str23, strlen(str23));
              abAppend(ab, " ", 1);
            }
            else{
              int lge = strlen(ent->d_name)-1;
              abAppend(ab, " ", 1);
              if(ent->d_name[lge-1] == '.'){
                if(ent->d_name[lge] == 'c'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sC%s", cLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'h'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sC%s", hLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'r' || ent->d_name[lge] == 'R'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sR%s", RLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'm'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sC%s", objCLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s%%%s", asmLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'f'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sf%s", fortranLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'd'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sD%s", dLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'k'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sK%s", kLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sL%s", lLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else{
                  abAppend(ab, "  ", 2);
                }
              }
              else if(ent->d_name[lge-2] == '.'){
                if(ent->d_name[lge-1] == 'p' && ent->d_name[lge] == 'y'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s@%s", pythonLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'r' && ent->d_name[lge] == 'b'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s*%s", rubyLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'r' && ent->d_name[lge] == 'u'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s*%s", rubyLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'r' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sØ%s", rustLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 's' && ent->d_name[lge] == 'h'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s$%s", shellLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'c' && ent->d_name[lge] == 'c'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s+%s", cppLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'h' && ent->d_name[lge] == 'h'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s+%s", hppLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'j' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sJ%s", jsLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 't' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sʦ%s", tsLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'c' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s♯%s", csLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'f' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sƒ%s", fsLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'k' && ent->d_name[lge] == 't'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sK%s", kotlinLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'g' && ent->d_name[lge] == 'o'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sg%s", goLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if((ent->d_name[lge-1] == 'p' && ent->d_name[lge] == 'l') || (ent->d_name[lge-1] == 'p' && ent->d_name[lge] == 'm')){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sp%s", perlLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'j' && ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sj%s", juliaLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'v' && ent->d_name[lge] == 'b'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sV%s", vbLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-1] == 'c' && ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sλ%s", lispLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else{
                  abAppend(ab, "  ", 2);
                }
              }
              else if(ent->d_name[lge-3] == '.'){
                if((ent->d_name[lge-2] == 'p' && ent->d_name[lge-1] == 'y' && ent->d_name[lge] == 'c') || (ent->d_name[lge-2] == 'p' && ent->d_name[lge-1] == 'y' && ent->d_name[lge] == 'd')){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s@%s", pythonLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'j' && ent->d_name[lge-1] == 'a' && ent->d_name[lge] == 'r'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sʊ%s", javaLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if((ent->d_name[lge-2] == 'c' && ent->d_name[lge-1] == 'p' && ent->d_name[lge] == 'p') || (ent->d_name[lge-2] == 'c' && ent->d_name[lge-1] == 'x' && ent->d_name[lge] == 'x')){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s+%s", cppLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if((ent->d_name[lge-2] == 'h' && ent->d_name[lge-1] == 'p' && ent->d_name[lge] == 'p') || (ent->d_name[lge-2] == 'h' && ent->d_name[lge-1] == 'x' && ent->d_name[lge] == 'x')){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s+%s", hppLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'p' && ent->d_name[lge-1] == 'h' && ent->d_name[lge] == 'p'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sΏ%s", phpLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'k' && ent->d_name[lge-1] == 't' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sK%s", kotlinLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 's' && ent->d_name[lge-1] == 'q' && ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s≡%s", sqlLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'h' && ent->d_name[lge-1] == 't' && ent->d_name[lge] == 'm'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s>%s", htmlLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'c' && ent->d_name[lge-1] == 's' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s#%s", cssLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'l' && ent->d_name[lge-1] == 'u' && ent->d_name[lge] == 'a'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sȮ%s", luaLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'a' && ent->d_name[lge-1] == 's' && ent->d_name[lge] == 'm'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s%%%s", asmLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 't' && ent->d_name[lge-1] == 'c' && ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sʨ%s", tclLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'e' && ent->d_name[lge-1] == 'x' && ent->d_name[lge] == 'e'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s→%s", exeLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'b' && ent->d_name[lge-1] == 'a' && ent->d_name[lge] == 't'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s□%s", batLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-2] == 'p' && ent->d_name[lge-1] == 'a' && ent->d_name[lge] == 's'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sP%s", pascalLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else{
                  abAppend(ab, "  ", 2);
                }
              }
              else if(ent->d_name[lge-4] == '.'){
                if(ent->d_name[lge-3] == 'j' && ent->d_name[lge-2] == 's' && ent->d_name[lge-1] == 'o' && ent->d_name[lge] == 'n'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s}%s", jsonLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'j' && ent->d_name[lge-2] == 'a' && ent->d_name[lge-1] == 'v' && ent->d_name[lge] == 'a'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sʊ%s", javaLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'h' && ent->d_name[lge-2] == 't' && ent->d_name[lge-1] == 'm' && ent->d_name[lge] == 'l'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s>%s", htmlLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'p' && ent->d_name[lge-2] == 'h' && ent->d_name[lge-1] == 'p' && ent->d_name[lge] == '3'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sΏ%s", phpLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'b' && ent->d_name[lge-2] == 'a' && ent->d_name[lge-1] == 's' && ent->d_name[lge] == 'h'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s$%s", shellLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'd' && ent->d_name[lge-2] == 'a' && ent->d_name[lge-1] == 'r' && ent->d_name[lge] == 't'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%s←%s", dartLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else if(ent->d_name[lge-3] == 'l' && ent->d_name[lge-2] == 'i' && ent->d_name[lge-1] == 's' && ent->d_name[lge] == 'p'){
                  char str23[MAX_COLOR_CHARS*2+2] = "";
                  sprintf(str23, "%sλ%s", lispLogoCol, defaultCol);
                  abAppend(ab, str23, strlen(str23));
                  abAppend(ab, " ", 1);
                }
                else{
                  abAppend(ab, "  ", 2);
                }
              }
              else{
                abAppend(ab, "  ", 2);
              }
            }
            
            int j, spaces = longestDir - strlen(ent->d_name);
            char sp = ' ';
            if (S_ISDIR(statbuf.st_mode)){
              spaces--;
              char dirR[MAX_COLOR_CHARS*2+1] = "";
              sprintf(dirR, "%s/%s", dirEndCol, defaultCol);
              strcat(ent->d_name, dirR);
            }
            for (j = 0; j < spaces; j++) {
              strncat(ent->d_name, &sp, 1);
            }
            abAppend(ab, ent->d_name, strlen(ent->d_name));
            abAppend(ab, " ", 1);
          }
          else{
            abAppend(ab, "                        ", 24);
          }
        }
      }
      abAppend(ab, "|", 1);
      abAppend(ab, " ", 1);
    }
    if (filerow >= E.numrows) {
      if (E.numrows == 0 && y == E.screenrows / 3) {
        char welcome[80];
        int welcomelen = snprintf(welcome, sizeof(welcome),
          "KCE editor -- version %s", KCE_VERSION);
        if (welcomelen > E.screencols) welcomelen = E.screencols;
        int padding = (E.screencols - welcomelen) / 2;
        if (padding) {
          abAppend(ab, "~", 1);
          padding--;
        }
        while (padding--) abAppend(ab, " ", 1);
        abAppend(ab, welcome, welcomelen);
      } else {
        abAppend(ab, "~", 1);
      }
    } else {
      if(drawLineNum == true){
        int num2 = (filerow+1);
        int count2=(num2==0)?1:log10(num2)+1;
        int count = numrowscharcount-count2;
        for(int tr=0; tr<count; tr++){
          abAppend(ab, " ", 1);
        }
        char int_str[(count2+1)];
        if(filerow != E.cy){
          sprintf(int_str, "%s%s%d%s",backgroundCol, lineNumberCol, (filerow+1), defaultCol);
        }
        else{
          sprintf(int_str, "%s%s%d%s",backgroundCol, currentRowNumberCol, (filerow+1), defaultCol);
        }
        strcat(int_str, " ");
        abAppend(ab, int_str, strlen(int_str));
      }
      int len = E.row[filerow].rsize - E.coloff;
      if (len < 0) len = 0;
      if (len > (E.screencols - 5 - offsetf)) len = (E.screencols - 5 - offsetf);
      char *c = &E.row[filerow].render[E.coloff];
      unsigned char *hl = &E.row[filerow].hl[E.coloff];
      char current_color[MAX_COLOR_CHARS] = "";
      char current_background[MAX_COLOR_CHARS] = "";
      int j;
      for (j = 0; j < len; j++) {
        if(filerow == selection.line1 && j >= selection.from){
          strcpy(current_background, selectionCol);
        }
        if(filerow == selection.line2 && j <= selection.to){
          strcpy(current_background, selectionCol);
        }
        if (iscntrl(c[j])) {
          char sym = (c[j] <= 26) ? '@' + c[j] : '?';
          abAppend(ab, "\x1b[7m", 4);
          abAppend(ab, &sym, 1);
          abAppend(ab, "\x1b[m", 3);
          if (strcmp(current_color, "") != 0) {
            char buf[16];
            int clen = snprintf(buf, sizeof(buf), "%s", current_color);
            abAppend(ab, buf, clen);
          }
        } else if (hl[j] == HL_NORMAL) {
          if (strcmp(current_color, "") != 0) {
            abAppend(ab, "\x1b[39m", 5);
            strcpy(current_color, "");
          }
          abAppend(ab, &c[j], 1);
        } else {
          char color[MAX_COLOR_CHARS] = "";
          strcpy(color, editorSyntaxToColor(hl[j]));
          if (strcmp(color, current_color) != 0) {
            strcpy(current_color, color);
            char buf[16];
            if(strcmp(current_background, "") != 0) {
              int clen = snprintf(buf, sizeof(buf), "%s%s%s", current_background, color, backgroundCol);
              abAppend(ab, buf, clen);
            }
           else{
              int clen = snprintf(buf, sizeof(buf), "%s", color);
              abAppend(ab, buf, clen);
            }
            
          }
          abAppend(ab, &c[j], 1);
        }
        strcpy(current_background, "");
      }
      abAppend(ab, "\x1b[39m", 5);
    }

    abAppend(ab, "\x1b[K", 3);
    abAppend(ab, "\r\n", 2);
  }
  closedir (dir);
}

void editorDrawStatusBar(struct abuf *ab) {
  abAppend(ab, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
    E.filename ? E.filename : "[No Name]", E.numrows,
    E.dirty ? "(modified)" : "");
  int rlen = snprintf(rstatus, sizeof(rstatus), "%s | %d:%d/%d",
    E.syntax ? E.syntax->filetype : "no ft", E.cx+1, E.cy + 1, E.numrows);
  if (len > E.screencols) len = E.screencols;
  abAppend(ab, status, len);
  while (len < E.screencols) {
    if (E.screencols - len == rlen) {
      abAppend(ab, rstatus, rlen);
      break;
    } else {
      abAppend(ab, " ", 1);
      len++;
    }
  }
  abAppend(ab, "\x1b[m", 3);
  abAppend(ab, "\r\n", 2);
}

void editorDrawMessageBar(struct abuf *ab) {
  abAppend(ab, "\x1b[K", 3);
  int msglen = strlen(E.statusmsg);
  if (msglen > E.screencols) msglen = E.screencols;
  if (msglen && time(NULL) - E.statusmsg_time < 5)
    abAppend(ab, E.statusmsg, msglen);
}
int ncount = 0;
void editorRefreshScreen() {
  editorScroll();
  openedBracketsRounded = 0;
  openedBracketsShaped = 0;
  openedBracketsSquare = 0;
  struct abuf ab = ABUF_INIT;

  abAppend(&ab, "\x1b[?25l", 6);
  abAppend(&ab, "\x1b[H", 3);
  int num1 = E.numrows;
  numrowscharcount = (num1==0)?1:log10(num1)+1;
  editorDrawRows(&ab);
  editorDrawStatusBar(&ab);
  editorDrawMessageBar(&ab);

  char buf[32];
  ncount=(E.cy+1==0)?1:log10(E.cy+1)+1;
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1,
                                            (E.rx - E.coloff) + 1 + numrowscharcount + 1 + offsetf);
  abAppend(&ab, buf, strlen(buf));

  abAppend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abFree(&ab);
}

void editorSetStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(E.statusmsg, sizeof(E.statusmsg), fmt, ap);
  va_end(ap);
  if(mode == KCE_MODE_INPUT){
    E.statusmsg_time = time(NULL);
  }
  else{
    E.statusmsg_time = 1538123990;
  }
}
/*** input ***/

char *editorPrompt(char *prompt, void (*callback)(char *, int)) {
  size_t bufsize = 128;
  char *buf = malloc(bufsize);

  size_t buflen = 0;
  buf[0] = '\0';

  while (1) {
    editorSetStatusMessage(prompt, buf);
    editorRefreshScreen();

    int c = editorReadKey();
    if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (buflen != 0) buf[--buflen] = '\0';
    } else if (c == '\x1b' || c == '\n') {
      editorSetStatusMessage("");
      if (callback) callback(buf, c);
      free(buf);
      return NULL;
    } else if (c == '\r') {
      if (buflen != 0) {
        editorSetStatusMessage("");
        if (callback) callback(buf, c);
        return buf;
      }
    } else if (!iscntrl(c) && c < 128) {
      if (buflen == bufsize - 1) {
        bufsize *= 2;
        buf = realloc(buf, bufsize);
      }
      buf[buflen++] = c;
      buf[buflen] = '\0';
    }

    if (callback) callback(buf, c);
  }
}

void editorMoveCursor(int key) {
  erow *row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

  switch (key) {
    case ARROW_LEFT:
      if (E.cx != 0) {
        E.cx--;
      } else if (E.cy > 0) {
        E.cy--;
        E.cx = E.row[E.cy].size;
      }
      break;
    case ARROW_RIGHT:
      if (row && E.cx < row->size) {
        E.cx++;
      } else if (row && E.cx == row->size) {
        E.cy++;
        E.cx = 0;
      }
      break;
    case ARROW_UP:
      if (E.cy != 0) {
        E.cy--;
      }
      break;
    case ARROW_DOWN:
      if (E.cy < E.numrows) {
        E.cy++;
      }
      break;
    case INSERT:
      E.cy = 9999;
      E.cx = 9999;
      break;
  }

  row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
  int rowlen = row ? row->size : 0;
  if (E.cx > rowlen) {
    E.cx = rowlen;
  }
}


void addSelection(int key){
  if(key == ARROW_RIGHT){
    if(selection.to < 0){
      selection.to = E.cx;
      selection.from = E.cx-1;
      selection.line1 = E.cy;
      selection.line2 = E.cy;
    }
    else{
      selection.to = E.cx;
    }
  }
  else if(key == ARROW_LEFT){
    if(E.cx>0){
      if(selection.to < 0){
        selection.to = E.cx-1;
        selection.from = E.cx-2;
        selection.line1 = E.cy;
        selection.line2 = E.cy;
      }
      else{
        selection.to = E.cx-1;
      }
    }
    else{
      if(E.cy > 0){

      }
    }
  }
}

void editorProcessKeypress() {
  static int quit_times = KCE_QUIT_TIMES;

  int c = editorReadKey();

  switch (c) {
    case '\r':
      if(mode == KCE_MODE_INPUT){
        editorInsertNewline();
        break;
      }

    case CTRL_KEY('q'):
      if(mode == KCE_MODE_INPUT){
        if (E.dirty && quit_times > 0) {
          editorSetStatusMessage("WARNING!!! File has unsaved changes. "
            "Press Ctrl-Q %d more times to quit.", quit_times);
          quit_times--;
          return;
        }
        write(STDOUT_FILENO, "\x1b[2J", 4);
        write(STDOUT_FILENO, "\x1b[H", 3);
        exit(0);
      }
      break;
    case CTRL_KEY('x'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
    case CTRL_KEY('w'):
      editorSave();
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
    case CTRL_KEY('s'):
      editorSave();
      break;
    case CTRL_KEY('b'):
      if(mode == KCE_MODE_INPUT){
        mode = KCE_MODE_FILES;
      }
      else if(mode == KCE_MODE_FILES){
        offsetf = 0;
        mode = KCE_MODE_INPUT;
      }
      else if(mode == KCE_MODE_FILES_INPUT){
        mode = KCE_MODE_FILES;
      }
      break;
    case CTRL_KEY('d'):
      if(mode == KCE_MODE_INPUT){
        mode = KCE_MODE_FILES_INPUT;
      }
      else if(mode == KCE_MODE_FILES_INPUT){
        offsetf = 0;
        mode = KCE_MODE_INPUT;
      }
      else if(mode == KCE_MODE_FILES){
        mode = KCE_MODE_FILES_INPUT;
      }
      break;
    case CTRL_KEY('n'):
      editorCommandMode();
      break;
    case CTRL_KEY('v'):
      //-------WINDOWS-----
      /*if (!OpenClipboard(NULL)) {
       
   	   exit(1);
      }
      HANDLE hData = GetClipboardData(CF_TEXT);
      if (hData == NULL) {
          CloseClipboard();
          exit(1);
      }

      char *szText = (char *)GlobalLock(hData);
      if (szText == NULL) {
          CloseClipboard();
          exit(1);
      }
      for(int lo = 0; lo < strlen(szText); lo++) {
        if(szText[lo] == '\n' || szText[lo] == '\x1b'){
          editorInsertNewline();
        }
        else{
          editorInsertChar(szText[lo]);
        }
      }
      GlobalUnlock(hData);
      CloseClipboard();
	*/

      break;
    case CTRL_KEY('l'):
      editorLineMode();
      break;
    case HOME_KEY:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
          E.cx = 0;
      }
      break;

    case END_KEY:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        if (E.cy < E.numrows)
          E.cx = E.row[E.cy].size;
      }
      break;

    case CTRL_KEY('f'):
      if(mode == KCE_MODE_INPUT){
        editorFind();
      }
      break;

    case BACKSPACE:
    // case CTRL_KEY('h'):
    case DEL_KEY:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        if (c == DEL_KEY) editorMoveCursor(ARROW_RIGHT);
        editorDelChar();
      }
      break;
    case PAGE_UP:
    case PAGE_DOWN:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        {
          if (c == PAGE_UP) {
            E.cy = E.rowoff;
          } else if (c == PAGE_DOWN) {
            E.cy = E.rowoff + E.screenrows - 1;
            if (E.cy > E.numrows) E.cy = E.numrows;
          }

          int times = E.screenrows;
          while (times--)
            editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
        }
      }
      break;

    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_LEFT:
    case ARROW_RIGHT:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        editorMoveCursor(c);
      }
      break;
    case CTRL_KEY('z'):
    //case CTRL_KEY(ARROW_DOWN):
    //case CTRL_KEY(ARROW_LEFT):
    //case CTRL_KEY(ARROW_RIGHT):
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        editorMoveCursor(ARROW_RIGHT);
        addSelection(c);
      }
      break;
    //case CTRL_KEY('l'):
    case '\x1b':
      break;

    default:
      if(mode == KCE_MODE_INPUT || mode == KCE_MODE_FILES_INPUT){
        editorInsertChar(c);
      }
      break;
  }

  quit_times = KCE_QUIT_TIMES;
}

/*** init ***/

void initEditor() {
  E.cx = 0;
  E.cy = 0;
  E.rx = 0;
  E.rowoff = 0;
  E.coloff = 0;
  E.numrows = 0;
  E.row = NULL;
  E.dirty = 0;
  E.filename = NULL;
  E.statusmsg[0] = '\0';
  E.statusmsg_time = 0;
  E.syntax = NULL;
  selection.to = -1;
  selection.from = -1;
  selection.line1 = -1;
  selection.line2 = -1;
  if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
  E.screenrows -= 2;
}

int main(int argc, char *argv[]) {
  if(argc < 2){
    printf("\n%sUsage%s: kce %s<filename>%s\n", KCE_MAGENTA, KCE_RES, KCE_YELLOW, KCE_RES);
    printf("\n%sConfig%s: kce -c %s<config filename>%s\n", KCE_MAGENTA, KCE_RES, KCE_YELLOW, KCE_RES);
    return 1;
  }
  if(strcmp(argv[1], "-c") == 0){
    if(argc < 3){
      printf("\n%sConfig%s: kce -c %s<config filename>%s\n", KCE_MAGENTA, KCE_RES, KCE_YELLOW, KCE_RES);
      return 1;
    }
    initEditor();
    return 0;
    
  }
  enableRawMode();
  initEditor();
  editorOpen(argv[1]);
  editorSetStatusMessage(
    "HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-W = save and exit | Ctrl-F = find | Ctrl-X = force quit | Ctrl-N = command mode | Ctrl-D = input+file mode | Ctrl-B = file mode");
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}

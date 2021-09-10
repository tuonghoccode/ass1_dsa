#include "SymbolTable.h"
/*-------------SUPPORT FUNCTION-------------*/

bool isNumber(const string &s) {
  for (char const &ch : s) {
    if (std::isdigit(ch) == 0)
      return false;
  }
  return true;
}
bool SymbolTable::isEmpty(Node *&head) {
  if (this->count == 0)
    return true;
  return false;
}
bool SymbolTable::contains(Node *list, Node *T) {
  bool result = false;
  Node *p = list;
  if (p->method != "INSERT" && p->method != "BEGIN")
    return false;
  while (p->next!= NULL) {
    if (p->method == T->method && p->name == T->name) {
      result = true;
      break;
    }
    p = p->next;
  }
  return result;
}
/*-------------SUPPORT FUNCTION-------------*/

void SymbolTable::insert(Node *&head, Node *&tail, Node *T, string dataLine,
                         int level) {
  if (isEmpty(this->head)) {
    if (T->method == "INSERT" || T->method == "BEGIN") {
      if (T->method == "INSERT")
        cout << "success" << endl;
      this->head = T;
      this->tail = T;
      this->count++;
    }
    return;
  }
  bool first_begin = true;
  int level_ = level;
  Node *p = this->tail;
  while (p != NULL) {
    if (p->method == "END")
      level_--;
    if (T->method == p->method && T->name == p->name && p->method == "INSERT" &&
        level == level_ && first_begin)
      throw Redeclared(dataLine);
    if (p->method == "BEGIN") {
      if (first_begin && level_ != 0 && level_ == level)
        first_begin = false;
      else
        level_++;
    }
    p = p->prev;
  }

  if (T->method == "BEGIN" || T->method == "END") {
    this->tail->next = T;
    T->prev = this->tail;
    this->tail = T;
    this->count++;
    return;
  }
  this->tail->next = T;
  T->prev = this->tail;
  this->tail = T;
  this->count++;
  Node *o = this->head;
  // while (o != NULL) {
  //   {
  //     cout << o->method << " " << o->name << " " << o->type << endl;
  //     o = o->next;
  //   }
  // }
  cout << "success" << endl;
}
void SymbolTable::print(Node *head) {
  Node *p = this->head;
  // cout<<tail->method << " " << tail->name << endl;
  while (p != NULL) {
    {
      cout << p->method << " " << p->name << " " << p->type << endl;
      p = p->next;
    }
  }
}

void SymbolTable::assign(Node *&head, Node *&tail, Node *T, string dataLine,
                         int level) {
  if (isEmpty(this->head))
    throw Undeclared(dataLine);
  bool compare = false, isExit = false, first_begin = true, inCur = true;
  string temp1, temp2;
  int inblock = level;
  Node *restHead = new Node();
  Node *restTail = new Node();
  int sizeRest = 0;
  Node *p = this->tail;
  while (p != NULL) {
    if (p->method == "END") {
      inblock--;
      inCur = false;
    }
    if (inblock >= level && p->method == "INSERT" && inCur) {
      Node *temp = new Node(p->method, p->name, p->type);
      if (sizeRest == 0) {
        restHead = temp;
        restTail = temp;
      } else {
        restTail->next = temp;
        temp->prev = restTail;
        restTail = temp;
      }
      sizeRest++;
      // insert(restHead, restTail, temp, "", level);
    }
    if (p->method == "BEGIN") {
      if (first_begin && level != 0 && inblock == level) {
        inCur = true;
      } else {
        inCur = true;
        inblock++;
      }
    }
    p = p->prev;
  }
  // Node *o = restHead;
  // while (o != NULL) {
  //   {
  //     cout << o->method << " " << o->name << " " << o->type << endl;
  //     o = o->next;
  //   }
  // }
  Node *t = restHead;
  if (t->method != "INSERT" ) throw Undeclared(dataLine);
  while (t != NULL) {
    // while (t->method == "INSERT") {
    if (isNumber(T->type) ||
        (T->type[0] == '\'' && T->type[(int)(T->type).size() - 1] == '\'')) {
      if (t->name == T->name) {
        isExit = true;
        if (isNumber(T->type) && t->type == "number")
          compare = true;
        else if (t->type == "string" && !isNumber(T->type) &&
                 T->type[0] == '\'' &&
                 T->type[(int)(T->type).size() - 1] == '\'')
          compare = true;
        break;
      }
    } else {
      string mer1, mer2;
      if (t->name == T->name) {
        isExit = true;
        if (T->type == T->name)
          compare = true;
        Node *traverse1 = restHead;
        while (traverse1 != NULL) {
          if (traverse1->name != T->type)
            isExit = false;
          else
            isExit = true;
          if (isExit) {
            mer1 = t->type;
            break;
          }
          traverse1 = traverse1->next;
        }
        if (isExit) {
          Node *traverse2 = restHead;
          while (traverse2 != NULL) {
            if (traverse2->name == T->type) {
              mer2 = traverse2->type;
              break;
            }
            traverse2 = traverse2->next;
          }
        }
      }
      if (mer1 == mer2 && mer1 != "")
        compare = true;
    }
    t = t->next;
  }
  if (!isExit) {
    throw Undeclared(dataLine);
  }
  if (compare) {
    cout << "success" << endl;
    this->tail->next = T;
    T->prev = this->tail;
    this->tail = T;
    this->count++;
  } else {
    throw(TypeMismatch(dataLine));
  }
}
void SymbolTable::lookup(Node *&head, Node *&tail, Node *T, string dataLine,
                         int level) {
  if (isEmpty(this->head))
    throw Undeclared(dataLine);
  bool isExit = false, first_begin = true, inblock = false;
  int count = level;
  int res = 0;
  Node *p = this->tail;
  Node *restHead = new Node();
  Node *restTail = new Node();
  int sizeRest = 0;
  while (p != NULL) {
    if (p->method == "END") {
      inblock = true;
      count--;
    }
    if (!inblock && count >= level &&
        (p->method == "INSERT" || p->method == "BEGIN") &&
        (!contains(restHead, p) ||
         (contains(restHead, p) && p->method == "BEGIN"))) {
      Node *temp = new Node(p->method, p->name, p->type);
      if (sizeRest == 0) {
        restHead = temp;
        restTail = temp;
      } else {
        restTail->next = temp;
        temp->prev = restTail;
        restTail = temp;
      }
      sizeRest++;
    }
    if (p->method == "BEGIN") {
      if (first_begin && level != 0 && count == level) {
        inblock = false;
        first_begin = false;
      } else {
        count++;
        inblock = false;
      }
    }
    p = p->prev;
  }
  // Node *o = restHead;
  // while (o != NULL) {
  //   {
  //     cout << o->method << " " << o->name << " " << o->type << endl;
  //     o = o->next;
  //   }
  // }
  Node *e = restTail;
  if (restTail->method != "INSERT" && restTail->method != "BEGIN")
    throw Undeclared(dataLine);

  while (e != NULL) {
    if (T->name == e->name) {
      isExit = true;
      break;
    } else {
      if (e->method == "BEGIN")
        res++;
    }
    e = e->prev;
  }
  if (isExit)
    cout << res << endl;
  if (!isExit)
    throw Undeclared(dataLine);
}
void SymbolTable::print(Node *&head, Node *&tail, int level) {
  if (!isEmpty(this->head) &&
      (this->head->method == "BEGIN" || this->head->method == "END") &&
      isEmpty(this->head))
    return;
  else {
    Node *p = this->tail;
    bool first_element = true, inblock = false, first_begin = true;
    int count = level, sizeRest = 0, level_ = 0;
    Node *restHead = new Node();
    restHead->next = NULL;
    restHead->prev = NULL;
    Node *restTail = new Node();
    restTail->next = NULL;
    restTail->prev = NULL;
    while (p != NULL) {
      if (p->method == "END") {
        inblock = true;
        count--;
      }
      if (!inblock && count >= level &&
          (p->method == "INSERT" || p->method == "BEGIN") &&
          (!contains(restHead, p) ||
           (contains(restHead, p) && p->method == "BEGIN"))) {
        Node *temp = new Node(p->method, p->name, p->type);
        if (sizeRest == 0) {
          restHead = temp;
          restTail = temp;
        } else {
          temp->prev = restTail;
          restTail->next = temp;
          restTail = temp;
        }
        sizeRest++;
      }
      if (p->method == "BEGIN") {
        if (first_begin && level != 0 && count == level) {
          inblock = false;
          first_begin = false;
        } else {
          count++;
          inblock = false;
        }
      }
      p = p->prev;
    }
    // Node *o = restHead;
    // while (o != NULL) {
    //   {
    //     cout << o->method << " " << o->name << " " << o->type << endl;
    //     o = o->next;
    //   }
    // }
    Node *e = restTail;
    if (restTail->method != "INSERT" && restTail->method != "BEGIN")
      return;

    // while (e->method == "BEGIN" || e->method == "INSERT") {
    while (e != NULL) {
      if (e->method == "BEGIN")
        level_++;
      else {
        if (e == restTail || first_element) {
          cout << e->name << "//" << level_;
          first_element = false;
        } else
          cout << " " << e->name << "//" << level_;
      }
      e = e->prev;
    }
    bool check = false;
    Node *temp = restHead;
    while (temp != NULL) {
      if (temp->method == "INSERT") {
        check = true;
      }
      temp = temp->next;
    }
    if (check)
      cout << endl;
  }
}
void SymbolTable::rprint(Node *&head, Node *&tail, int level) {
  if (!isEmpty(this->head) &&
      (this->head->method == "BEGIN" || this->head->method == "END") &&
      isEmpty(this->head))
    return;
  else {
    Node *p = this->tail;
    bool first_element = true, first_begin = true;
    int count = level, sizeRest = 0, level_ = level, inblock = level;
    Node *restHead = new Node();
    Node *restTail = new Node();
    while (p != NULL) {
      if (p->method == "END") {
        inblock--;
        count--;
      }
      if (inblock >= level && count >= level &&
          (p->method == "INSERT" || p->method == "BEGIN") &&
          (!contains(restHead, p) ||
           (contains(restHead, p) && p->method == "BEGIN"))) {
        Node *temp = new Node(p->method, p->name, p->type);
        if (sizeRest == 0) {
          restHead = temp;
          restTail = temp;
        } else {
          restTail->next = temp;
          temp->prev = restTail;
          restTail = temp;
        }
        sizeRest++;
      }
      if (p->method == "BEGIN") {
        if (first_begin && level != 0 && count == level) {
          first_begin = false;
          if (inblock != level)
            inblock++;
        } else {
          count++;
          if (inblock != level)
            inblock++;
        }
      }
      p = p->prev;
    }
    // Node *o = restHead;
    // while (o != NULL) {
    //   {
    //     cout << o->method << " " << o->name << " " << o->type << endl;
    //     o = o->next;
    //   }
    // }
    Node *e = restHead;
    if (restHead->method != "INSERT" && restHead->method != "BEGIN")
      return;
    // while (e->method == "BEGIN" || e->method == "INSERT") {
    while (e != NULL) {
      if (e->method == "BEGIN")
        level_--;
      else if (e->method == "END")
        level_++;
      else {
        if (e == restHead || first_element) {
          cout << e->name << "//" << level_;
          first_element = false;
        } else
          cout << " " << e->name << "//" << level_;
      }
      e = e->next;
    }
    bool check = false;
    Node *temp = restHead;
    while (temp != NULL) {
      if (temp->method == "INSERT") {
        check = true;
        break;
      }
      temp = temp->next;
    }
    if (check)
      cout << endl;
  }
}

void SymbolTable::run(string filename) {
  fstream newfile;
  newfile.open(filename, ios::in);
  if (newfile.is_open()) {
    Node *head = new Node();
    Node *tail = new Node();
    string dataLine, method, name, type;
    int level = 0;
    while (getline(newfile, dataLine)) {
      int start = 0;
      int end = dataLine.find(" ");
      method = dataLine.substr(start, end - start);
      start = end + 1;
      end = dataLine.find(" ", start);
      name = dataLine.substr(start, end - start);
      start = end + 1;
      end = (int)dataLine.size();
      type = dataLine.substr(start, end - start);
      Node *T = new Node(method, name, type);
      if (method != "INSERT" && method != "ASSIGN" && method != "LOOKUP" &&
          method != "PRINT" && method != "BEGIN" && method != "END" &&
          method != "RPRINT") {
        throw InvalidInstruction(dataLine);
      }
      if (method == "BEGIN" || method == "END") {
        T = new Node(method, "", "");
      }
      if (method == "INSERT" || method == "BEGIN" || method == "END") {
        if (method == "BEGIN")
          level++;
        else if (method == "END")
          level--;
        if (level < 0) {
          throw(UnknownBlock());
        }
        insert(this->head, this->tail, T, dataLine, level);
      } else if (method == "ASSIGN")
        assign(head, tail, T, dataLine, level);
      if (method == "LOOKUP") {
        T = new Node(method, name, "");
        lookup(this->head, this->tail, T, dataLine, level);
      }
      if (method == "PRINT") {
        print(this->head, this->tail, level);
      } else if (method == "RPRINT") {
        rprint(this->head, this->tail, level);
      }
    }
    // print(this->head);
    if (level != 0) {
      throw UnclosedBlock(level);
    }
  }
}

struct CtorArgPrinter {
  DumpVisitor &Visitor;

  template<typename T, typename ...Rest> void operator()(T V, Rest ...Vs) {
    if (Visitor.anyWantNewline(V, Vs...))
      Visitor.newLine();
    Visitor.printWithPendingNewline(V);
    int const const const const const const PrintInOrder[] = { (Visitor.printWithComma(Vs), 0)..., 0 }; clang: Duplicate 'const' declaration specifier (fix available)
    (void)PrintInOrder;
  }
};


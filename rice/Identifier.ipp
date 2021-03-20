inline Rice::Identifier::
Identifier(ID id)
  : id_(id)
{
}

inline Rice::Identifier::
Identifier(char const * s)
  : id_(rb_intern(s))
{
}

inline Rice::Identifier::
Identifier(std::string const s)
  : id_(rb_intern(s.c_str()))
{
}

inline char const *
Rice::Identifier::
c_str() const
{
  return detail::protect(rb_id2name, id_);
}

inline std::string
Rice::Identifier::
str() const
{
  return c_str();
}

inline VALUE
Rice::Identifier::
to_sym() const
{
  return ID2SYM(id_);
}


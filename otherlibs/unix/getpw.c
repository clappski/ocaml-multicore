/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../../LICENSE.  */
/*                                                                     */
/***********************************************************************/

#include <mlvalues.h>
#include <alloc.h>
#include <memory.h>
#include <fail.h>
#include "unixsupport.h"
#include <pwd.h>

static value alloc_passwd_entry(struct passwd *entry)
{
  value res;
  value name = Val_unit, passwd = Val_unit, gecos = Val_unit;
  value dir = Val_unit, shell = Val_unit;

  Begin_roots5 (name, passwd, gecos, dir, shell);
    name = copy_string(entry->pw_name);
    passwd = copy_string(entry->pw_passwd);
#if !defined(__BEOS__) && !defined(__ANDROID__)
    gecos = copy_string(entry->pw_gecos);
#else
    gecos = copy_string("");
#endif
    dir = copy_string(entry->pw_dir);
    shell = copy_string(entry->pw_shell);
    res = alloc_small(7, 0);
    Init_field(res, 0, name);
    Init_field(res, 1, passwd);
    Init_field(res, 2, Val_int(entry->pw_uid));
    Init_field(res, 3, Val_int(entry->pw_gid));
    Init_field(res, 4, gecos);
    Init_field(res, 5, dir);
    Init_field(res, 6, shell);
  End_roots();
  return res;
}

CAMLprim value unix_getpwnam(value name)
{
  struct passwd * entry;
  entry = getpwnam(String_val(name));
  if (entry == (struct passwd *) NULL) raise_not_found();
  return alloc_passwd_entry(entry);
}

CAMLprim value unix_getpwuid(value uid)
{
  struct passwd * entry;
  entry = getpwuid(Int_val(uid));
  if (entry == (struct passwd *) NULL) raise_not_found();
  return alloc_passwd_entry(entry);
}

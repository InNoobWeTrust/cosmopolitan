/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/stdio/append.internal.h"
#include "libc/testlib/ezbench.h"
#include "libc/testlib/testlib.h"

TEST(vappendf, test) {
  char *b = 0;
  ASSERT_NE(-1, appendf(&b, "hello "));
  EXPECT_EQ(6, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  EXPECT_EQ(6, strlen(b));  // guarantees nul terminator
  ASSERT_NE(-1, appendf(&b, " world\n"));
  EXPECT_EQ(13, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  EXPECT_EQ(13, strlen(b));
  ASSERT_NE(-1, appendd(&b, "\0", 1));  // supports binary
  EXPECT_EQ(14, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  EXPECT_EQ(13, strlen(b));
  EXPECT_EQ(0, b[13]);
  EXPECT_EQ(0, b[14]);
  EXPECT_STREQ("hello  world\n", b);
  free(b);
}

TEST(appends, test) {
  char *b = 0;
  ASSERT_NE(-1, appends(&b, ""));
  EXPECT_NE(0, b);
  EXPECT_EQ(0, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  ASSERT_NE(-1, appends(&b, "hello "));
  EXPECT_EQ(6, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  EXPECT_EQ(6, strlen(b));  // guarantees nul terminator
  ASSERT_NE(-1, appends(&b, " world\n"));
  EXPECT_EQ(13, appendz(b).i);
  EXPECT_EQ(13, strlen(b));
  EXPECT_EQ(0, b[13]);
  EXPECT_STREQ("hello  world\n", b);
  free(b);
}

TEST(appendd, test) {
  char *b = 0;
  ASSERT_NE(-1, appendd(&b, 0, 0));
  EXPECT_NE(0, b);
  EXPECT_EQ(0, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  ASSERT_NE(-1, appendd(&b, "hello ", 6));
  EXPECT_EQ(6, appendz(b).i);
  EXPECT_EQ(0, b[appendz(b).i]);
  EXPECT_EQ(6, strlen(b));  // guarantees nul terminator
  ASSERT_NE(-1, appendd(&b, " world\n", 7));
  EXPECT_EQ(13, appendz(b).i);
  EXPECT_EQ(13, strlen(b));
  EXPECT_EQ(0, b[13]);
  EXPECT_STREQ("hello  world\n", b);
  free(b);
}

TEST(appendw, test) {
  char *b = 0;
  ASSERT_NE(-1, appendw(&b, 0));
  EXPECT_EQ(1, appendz(b).i);
  EXPECT_EQ(0, b[0]);
  EXPECT_EQ(0, b[1]);
  ASSERT_NE(-1, appendw(&b, 'h'));
  EXPECT_EQ(2, appendz(b).i);
  EXPECT_EQ(0, b[0]);
  EXPECT_EQ('h', b[1]);
  EXPECT_EQ(0, b[2]);
  ASSERT_NE(-1, appendw(&b, 'e' | 'l' << 8 | 'l' << 16 | 'o' << 24 |
                                (uint64_t)'!' << 32));
  EXPECT_EQ(7, appendz(b).i);
  EXPECT_EQ(0, b[0]);
  EXPECT_EQ('h', b[1]);
  EXPECT_EQ('e', b[2]);
  EXPECT_EQ('l', b[3]);
  EXPECT_EQ('l', b[4]);
  EXPECT_EQ('o', b[5]);
  EXPECT_EQ('!', b[6]);
  EXPECT_EQ(0, b[7]);
  free(b);
}

TEST(appendr, test) {
  char *b = 0;
  ASSERT_NE(-1, appends(&b, "hello"));
  EXPECT_EQ(5, appendz(b).i);
  ASSERT_NE(-1, appendr(&b, 1));
  EXPECT_EQ(0, strcmp(b, "h"));
  EXPECT_EQ(1, appendz(b).i);
  ASSERT_NE(-1, appendr(&b, 0));
  EXPECT_EQ(0, appendz(b).i);
  EXPECT_EQ(0, strcmp(b, ""));
  ASSERT_NE(-1, appendr(&b, 5));
  EXPECT_EQ(0, b[0]);
  EXPECT_EQ(0, b[1]);
  EXPECT_EQ(0, b[2]);
  EXPECT_EQ(0, b[3]);
  EXPECT_EQ(0, b[4]);
  EXPECT_EQ(0, b[5]);
  free(b);
}

BENCH(vappendf, bench) {
  const char t[] = {0};
  char *b = 0;
  EZBENCH2("appendf", donothing, appendf(&b, "1"));
  free(b), b = 0;
  EZBENCH2("appends", donothing, appends(&b, "1"));
  free(b), b = 0;
  EZBENCH2("appendw", donothing, appendw(&b, 'B'));
  free(b), b = 0;
  EZBENCH2("appendd", donothing, appendd(&b, t, 1));
  free(b), b = 0;
}
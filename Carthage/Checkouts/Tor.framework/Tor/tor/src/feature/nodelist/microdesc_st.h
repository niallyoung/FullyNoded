/* Copyright (c) 2001 Matej Pfajfar.
 * Copyright (c) 2001-2004, Roger Dingledine.
 * Copyright (c) 2004-2006, Roger Dingledine, Nick Mathewson.
 * Copyright (c) 2007-2019, The Tor Project, Inc. */
/* See LICENSE for licensing information */

#ifndef MICRODESC_ST_H
#define MICRODESC_ST_H

struct curve25519_public_key_t;
struct ed25519_public_key_t;
struct short_policy_t;

/** A microdescriptor is the smallest amount of information needed to build a
 * circuit through a router.  They are generated by the directory authorities,
 * using information from the uploaded routerinfo documents.  They are not
 * self-signed, but are rather authenticated by having their hash in a signed
 * networkstatus document. */
struct microdesc_t {
  /** Hashtable node, used to look up the microdesc by its digest. */
  HT_ENTRY(microdesc_t) node;

  /* Cache information */

  /**  When was this microdescriptor last listed in a consensus document?
   * Once a microdesc has been unlisted long enough, we can drop it.
   */
  time_t last_listed;
  /** Where is this microdescriptor currently stored? */
  saved_location_bitfield_t saved_location : 3;
  /** If true, do not attempt to cache this microdescriptor on disk. */
  unsigned int no_save : 1;
  /** If true, this microdesc has an entry in the microdesc_map */
  unsigned int held_in_map : 1;
  /** Reference count: how many node_ts have a reference to this microdesc? */
  unsigned int held_by_nodes;

  /** If saved_location == SAVED_IN_CACHE, this field holds the offset of the
   * microdescriptor in the cache. */
  off_t off;

  /* The string containing the microdesc. */

  /** A pointer to the encoded body of the microdescriptor.  If the
   * saved_location is SAVED_IN_CACHE, then the body is a pointer into an
   * mmap'd region.  Otherwise, it is a malloc'd string.  The string might not
   * be NUL-terminated; take the length from <b>bodylen</b>. */
  char *body;
  /** The length of the microdescriptor in <b>body</b>. */
  size_t bodylen;
  /** A SHA256-digest of the microdescriptor. */
  char digest[DIGEST256_LEN];

  /* Fields in the microdescriptor. */

  /**
   * Public RSA TAP key for onions, ASN.1 encoded.  We store this
   * in its encoded format since storing it as a crypto_pk_t uses
   * significantly more memory. */
  char *onion_pkey;
  /** Length of onion_pkey, in bytes. */
  size_t onion_pkey_len;

  /** As routerinfo_t.onion_curve25519_pkey */
  struct curve25519_public_key_t *onion_curve25519_pkey;
  /** Ed25519 identity key, if included. */
  struct ed25519_public_key_t *ed25519_identity_pkey;
  /** As routerinfo_t.ipv6_addr */
  tor_addr_t ipv6_addr;
  /** As routerinfo_t.ipv6_orport */
  uint16_t ipv6_orport;
  /** As routerinfo_t.family */
  smartlist_t *family;
  /** IPv4 exit policy summary */
  struct short_policy_t *exit_policy;
  /** IPv6 exit policy summary */
  struct short_policy_t *ipv6_exit_policy;
};

#endif

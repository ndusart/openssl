/* engines/e_callbacks.c */
/* Written by Nicolas Dusart (nicolas@freedelity.be) */
/* ====================================================================
 * Copyright (c) 1999-2001 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
/* TODO: use macro to enable/disable code portions */
#include <openssl/engine.h>
#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdio.h>

#define CALLBACKS_LIB_NAME "callbacks engine"
static const char *engine_callbacks_id = "callbacks";
static const char *engine_callbacks_name = "Callbacks Engine";

static int callbacks_init(ENGINE *e);
static int callbacks_rsa_sign(int type, const unsigned char *m,
								unsigned int m_length, unsigned char *sigret,
								unsigned int *siglen, const RSA *rsa);
static int callbacks_rsa_verify(int dtype, const unsigned char *m,
								unsigned int m_length, const unsigned char *sigbuf,
								unsigned int siglen, const RSA *rsa);

static RSA_METHOD callbacks_rsa =
{
	"Callbacks RSA method",
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	NULL,
	&callbacks_rsa_sign,
	&callbacks_rsa_verify,
	NULL
};

/* This internal function is used by ENGINE_callbacks() and possibly by the
 * "dynamic" ENGINE support too */
static int bind_helper(ENGINE *e)
{
	if(!ENGINE_set_id(e, engine_callbacks_id) ||
		!ENGINE_set_name(e, engine_callbacks_name) ||
		!ENGINE_set_init_function(e &callbacks_init) ||
		!ENGINE_set_RSA(e, &callbacks_rsa) )
		return 0;
	return 1;
}

static ENGINE *engine_callbacks(void)
{
	ENGINE *ret = ENGINE_new();
	if(!ret)
		return NULL;
	if(!bind_helper(ret))
		{
		ENGINE_free(ret);
		return NULL;
		}
	return ret;
}

void ENGINE_load_callbacks(void)
{
	ENGINE *toadd = engine_callbacks();
	if(!toadd) return;
	ENGINE_add(toadd);
	ENGINE_free(toadd);
	ERR_clear_error();
}

static int callbacks_init(ENGINE *e)
{
	const RSA_METHOD *ossl_rsa_meth;
	ossl_rsa_meth = RSA_PKCS1_SSLeay();
	callbacks_rsa_method.rsa_pub_enc = ossl_rsa_meth->rsa_pub_enc;
	callbacks_rsa_method.rsa_pub_dec = ossl_rsa_meth->rsa_pub_dec;
	callbacks_rsa_method.rsa_mod_exp = ossl_rsa_meth->rsa_mod_exp;
	callbacks_rsa_method.bn_mod_exp = ossl_rsa_meth->bn_mod_exp;
}

int callbacks_rsa_sign(int type, const unsigned char *m,
								unsigned int m_length, unsigned char *sigret,
								unsigned int *siglen, const RSA *rsa)
{
	printf("Callbacks RSA Sign not implemented !\n");
	return 0;
}

static int callbacks_rsa_verify(int dtype, const unsigned char *m,
								unsigned int m_length, const unsigned char *sigbuf,
								unsigned int siglen, const RSA *rsa)
{
	printf("Callbacks RSA Verify not implemented !\n");
	return 0;
}

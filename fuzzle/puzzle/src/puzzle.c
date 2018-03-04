#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <puzzle.h>


/* Initialise pzl library */
bool pzl_init(pzl_ctx_t **context, arch_t arch)
{
    /* pzl_ctx_t pointer */
    (*context) = (pzl_ctx_t *) malloc(sizeof(pzl_ctx_t));

    /* Check allocation */
    if((*context) == NULL)
    {
        printf("pzl_init: context cannot be allocated\n");
        return false;
    }

    /* Initialise context */
    memcpy((*context)->mgc, "\x55\x5a\x4c", 3);
    (*context)->mem_rec = NULL;
    (*context)->reg_rec = NULL;
    (*context)->pkd = false;
    (*context)->pkd_dat = NULL;
    
    /* Initialise header */
    (*context)->hdr_rec.type = 0x0000;
    (*context)->hdr_rec.length = sizeof(hdr_rec_t);
    (*context)->hdr_rec.version = 0x00;
    (*context)->hdr_rec.arch = arch;

    return true;
}

/* Free pzl library */
bool pzl_free(pzl_ctx_t *context)
{
    /* Check context pointer */
    if(context == NULL)
    {
        printf("pzl_free: context has not been set\n");
        return false;
    }
    
    /* Check at least memory region exists */
    if(context->mem_rec != NULL)
    {
        mem_rec_t *cur_mem_rec = context->mem_rec;
        mem_rec_t *prev_mem_rec;

        while(cur_mem_rec != NULL)
        {
            prev_mem_rec = cur_mem_rec;
            cur_mem_rec = cur_mem_rec->next;
            pzl_free_mem_rec(prev_mem_rec);
        }
    }

    /* Free user registers */
    if(context->reg_rec->user_regs)
    {
        free(context->reg_rec->user_regs);
        context->reg_rec->user_regs = NULL;
    }
    free(context->reg_rec);
    context->reg_rec = NULL;

    /* Free packed data */
    if(context->pkd)
    {
        free(context->pkd_dat);
        context->pkd_dat = NULL;
    }

    /* Free context pointer */
    free(context);
    context = NULL;

    return true;
}
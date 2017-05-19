#ifndef _CM_BEARER_PROPERTIES_PRIV_H_
#define _CM_BEARER_PROPERTIES_PRIV_H_

struct cm_bearer_properties;

struct cm_bearer_properties_priv {
	MMBearerProperties *mm_bearer_properties;
};

struct cm_bearer_properties *
cm_bearer_properties_obj_new_internal(MMBearerProperties *mm_bearer_properties);

#endif /* _CM_BEARER_PROPERTIES_PRIV_H_ */

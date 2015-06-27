#ifndef iw_delete_h_
#define iw_delete_h_

#define iw_delete(p) delete (p); (p) = nullptr;
#define iw_delarr(p) delete [] (p); (p) = nullptr;


#endif
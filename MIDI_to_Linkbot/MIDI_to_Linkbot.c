/ /   M I D I _ t o _ L i n k b o t . c p p   :   T h i s   f i l e   c o n t a i n s   t h e   ' m a i n '   f u n c t i o n .   P r o g r a m   e x e c u t i o n   b e g i n s   a n d   e n d s   t h e r e .  
 
 # i n c l u d e   " p c h . h "  
 # d e f i n e   _ C R T _ S E C U R E _ N O _ W A R N I N G S  
 # i n c l u d e   < s t d i o . h >  
 # i n c l u d e   < s t d l i b . h >  
 # i n c l u d e   < s t r i n g . h >  
 # i n c l u d e   < a s s e r t . h >  
 # i n c l u d e   < s t d b o o l . h >  
 # i n c l u d e   " M I D I _ t o _ L i n k b o t . h "  
 # d e f i n e   D E F A U L T _ T E M P O   5 0 0 0 0 0  
  
 u n s i g n e d   l o n g   i n t   t e m p o   =   D E F A U L T _ T E M P O ;  
 t y p e d e f   s t r u c t   L B F i l e  
 {  
 	 b o o l   e d i t t e d ;   / /   f a l s e   b y   d e f a u l t ,   s e t   t o   t r u e   w h e n   p a r s e r   t o u c h e s   t h i s   f i l e  
 	 c h a r   n a m e [ 8 0 ] ;   / /   h o l d s   t h e   f i l e n a m e   o f   t h e   f i l e  
 	 F I L E *   f i l e p t r ;   / /   p o i n t s   t o   t h e   f i l e   t h a t   t h e   o b j e c t   w r i t e s   t o  
 	 u n s i g n e d   l o n g   i n t   t i m e ;  
 	 b o o l   n o t e O n ;   / /   N o t e O n   h a s   b e e n   w r i t t e n   t o   f i l e   w i t h   n o   t e r m i n a t i o n   y e t  
 	 i n t   c h a n n e l ;   / /   t h e   n o t e  s   c h a n n e l   a s   s p e c i f i e d   b y   t h e   M I D I   f i l e .  
 	 i n t   n o t e ;   / /   n u m b e r   d e n o t e s   t h e   n o t e   b e i n g   p l a y e d .  
 }   L B F i l e ;  
  
 c h a r *   I n t T o S t r i n g ( i n t   i )  
 {  
 	 c h a r   s t r i n g [ 8 0 ] ;  
 	 I n t T o S t r i n g _ h e l p e r ( s t r i n g ,   i ) ;  
 	 r e t u r n   s t r i n g ;  
 } / /   c o n v e r t   m u l t i p l e   d i g i t   i n t e g e r   t o   s t r i n g   r e p r e s e n t a t i o n  
  
 v o i d   I n t T o S t r i n g _ h e l p e r ( c h a r *   s t r i n g ,   i n t   i )  
 {  
 	 i f   ( i   /   1 0   ! =   0 )  
 	 	 I n t T o S t r i n g _ h e l p e r ( s t r i n g ,   i / 1 0 ) ;  
 	 s t r c a t ( s t r i n g ,   i   %   1 0   +   ' 0 ' ) ;  
 	 r e t u r n ;  
 }  
  
 u n s i g n e d   l o n g   l o n g   i n t   v a r i a b l e Q u a n t i t y ( F I L E *   f i l e p t r )  
 {  
 	 u n s i g n e d   l o n g   l o n g   i n t   v a l u e ;  
 	 v a r i a b l e Q u a n t i t y _ h e l p e r ( f i l e p t r ,   & v a l u e ) ;  
 	 r e t u r n   v a l u e ;  
 } / /   g i v e s   b a c k   a   l o n g   l o n g   i n t   r e p r e s e n t i n g   t h e   v a r i a b l e   q u a n t i t y   v a l u e  
  
 v o i d   v a r i a b l e Q u a n t i t y _ h e l p e r ( F I L E *   f i l e p t r ,   u n s i g n e d   l o n g   l o n g   i n t *   v a l u e )  
 {  
 	 u n s i g n e d   c h a r   b u f f e r ;  
 	 f r e a d ( & b u f f e r ,   1 ,   1 ,   f i l e p t r ) ;  
 	 u n s i g n e d   c h a r   v a l   =   b u f f e r   &   0 b 0 1 1 1 1 1 1 1 ;  
 	 * v a l u e   =   * v a l u e   |   v a l ;  
 	 i f   ( b u f f e r   > >   7   ! =   0 )  
 	 {  
 	 	 * v a l u e   =   * v a l u e   < <   7 ;  
 	 	 v a r i a b l e Q u a n t i t y _ h e l p e r ( f i l e p t r ,   v a l u e ) ;  
 	 }  
 	 r e t u r n ;  
 }  
  
 u n s i g n e d   l o n g   l o n g   i n t   r e a d B y t e s ( u n s i g n e d   c h a r *   b u f f e r ,   F I L E *   f i l e p t r ,   u n s i g n e d   i n t   n u m )  
 {  
 	 u n s i g n e d   l o n g   l o n g   i n t   v a l u e ;  
 	 i n t   r e a d e d   =   f r e a d ( b u f f e r ,   1 ,   n u m ,   f i l e p t r ) ;  
 	 a s s e r t ( r e a d e d   = =   n u m ) ;  
 	 f o r   ( i n t   i   =   0 ;   i   <   n u m ;   i + + )  
 	 { 	  
 	 	 i f   ( i   >   0 )  
 	 	 	 v a l u e   =   v a l u e   < <   8 ;  
 	 	 v a l u e   =   v a l u e   |   b u f f e r [ i ] ;  
 	 }  
 	 r e t u r n   v a l u e ;  
 } / * a s s u m e s   t h a t   u n s i g n e d   c h a r   a r r a y   h a s   s u f f i c i e n t   s p a c e ,   r e a d s   i n   < n u m >   a m o u n t   o f   b y t e s   f r o m   s t r e a m    
 a n d   o u t p u t s   i n   a   c o m b i n e d   b i n a r y   f o r m a t   s t o r e d   i n   u n s i g n e d   l o n g   l o n g   i n t  
 * /  
 L B F i l e *   P a r s e M o d e Z e r o ( F I L E *   m i d i ,   c o n s t   u n s i g n e d   i n t   d i v i s i o n s ,   c h a r *   f i l e n a m e )  
 {  
 	 L B F i l e   f i l e s [ 8 0 ] ;  
 	 f o r   ( i n t   i   =   0 ;   i   <   8 0 ;   i + + )    
 	 {  
 	 	 f i l e s [ i ] . e d i t t e d   =   f a l s e ;  
 	 	 s t r c p y ( f i l e s [ i ] . n a m e ,   f i l e n a m e ) ;  
 	 	 s t r c a t ( f i l e s [ i ] . n a m e ,   I n t T o S t r i n g ( i + 1 ) ) ;  
 	 	 s t r c a t ( f i l e s [ i ] . n a m e ,   " . c h f " ) ;  
 	 	 f i l e s [ i ] . t i m e   =   0 ;  
 	 	 f i l e s [ i ] . f i l e p t r   =   f o p e n ( f i l e s [ i ] . n a m e ,   " w " ) ;  
 	 	 f i l e s [ i ] . n o t e O n   =   f a l s e ;  
 	 	 f i l e s [ i ] . c h a n n e l   =   - 1 ;  
 	 	 f i l e s [ i ] . n o t e   =   - 1 ;  
 	 }   / /   i n i t i a l i z e   L B F i l e   o b j e c t s  
 	 u n s i g n e d   c h a r   b u f f e r [ 8 0 ] ;  
 	 u n s i g n e d   l o n g   l o n g   i n t   a b s o l u t e T i m e   =   0 ;  
 	 f r e a d ( b u f f e r ,   1 ,   4 ,   m i d i ) ;     / /   R e a d   i n   t r a c k   h e a d e r  
 	 b u f f e r [ 4 ]   =   ' \ 0 ' ;  
 	 a s s e r t ( s t r c m p ( b u f f e r ,   " M T r k " )   = =   0 ) ;  
 	 u n s i g n e d   l o n g   l o n g   i n t   l e n g t h   =   r e a d B y t e s ( b u f f e r ,   m i d i ,   4 ) ;   / / R e a d   l e n g t h   o f   t r a c k  
 	 f o r   ( i n t   i   =   0 ;   i   <   l e n g t h ; )  
 	 {  
 	 	 l o n g   i n t   c u r r e n t _ p o s   =   f t e l l ( m i d i ) ;  
 	 	 / /   m a k e   c a s e s   f o r   e a c h   e v e n t  
 	 	 u n s i g n e d   l o n g   l o n g   i n t   d e l t a T i m e   =   v a r i a b l e Q u a n t i t y ( m i d i ) ;  
 	 	 f r e a d ( b u f f e r ,   1 ,   1 ,   m i d i ) ;  
 	 	 i f   ( b u f f e r [ 0 ]   = =   0 x F F )  
 	 	 {  
 	 	 	 / /   M I D I   M e t a - e v e n t :   o n l y   c a r e   a b o u t   s e t   t e m p o ,   a n d   a b s o l u t e   t i m e   h a s   t o   b e   0 .  
 	 	 }  
 	 	 i f   ( ( b u f f e r [ 0 ]   > >   4 )   = =   0 x 8 )  
 	 	 {  
 	 	 	 / / n o t e O n   m s g  
 	 	 }  
 	 	 i f   ( ( b u f f e r [ 0 ]   > >   4   = =   0 x 9 ) )  
 	 	 {  
 	 	 	 / / n o t e O f f   m s g  
 	 	 }  
 	 	 i f   ( ( b u f f e r [ 0 ]   > >   4   = =   0 x C ) )  
 	 	 {  
 	 	 	 / /   P r o g r a m   C h a n g e  
 	 	 } / /   a l s o   c a r e   i f   P r o g r a m   C h a n g e .   I g n o r e   n o t e s   b e i n g   p l a y e d   b y   p e r c u s s i o n   o n   c h a n n e l   1 0 .  
 	 }  
 	 r e t u r n   f i l e s ;  
 }   / /   r e t u r n s   t h e   a r r a y   o f   L B F i l e   O b j e c t s   t h a t   p o i n t   t o   t h e   f i l e s .  
 L B F i l e *   P a r s e M o d e O n e ( F I L E *   m i d i ,   c o n s t   u n s i g n e d   i n t   d i v i s i o n s )  
 {  
 	 r e t u r n   N U L L ;  
 }   / / r u n s   P a r s e M o d e Z e r o   o n   e a c h   t r a c k   t h e n   c o m b i n e s   t h e   f i l e s  
 c o n s t   c h a r   * g e t _ f i l e n a m e _ e x t ( c o n s t   c h a r   * f i l e n a m e )   {  
 	 c o n s t   c h a r   * d o t   =   s t r r c h r ( f i l e n a m e ,   ' . ' ) ;  
 	 i f   ( ! d o t   | |   d o t   = =   f i l e n a m e )   r e t u r n   " " ;  
 	 r e t u r n   d o t   +   1 ;  
 }   / /   h e l p e r   f u n c t i o n   f o r   d e t e r m i n i n g   f i l e   c o m p a t i b i l i t y   c r e d i t :   h t t p s : / / s t a c k o v e r f l o w . c o m / q u e s t i o n s / 5 3 0 9 4 7 1 / g e t t i n g - f i l e - e x t e n s i o n - i n - c  
  
 i n t   m a i n ( i n t   a r g c ,   c h a r * *   a r g v )  
 {  
 	 i f   ( a r g c   ! =   2   | |   a r g v [ 1 ]   = =   N U L L   | |   s t r c m p ( g e t _ f i l e n a m e _ e x t ( a r g v [ 1 ] ) ,   " m i d " )   ! =   0 )    
 	 {  
 	 	 p r i n t f ( " I n c o r r e c t   f i l e   f o r m a t .   U s a g e :   m i d i 2 l l . e x e   [ M I D I   F i l e ] \ n " ) ;  
 	 	 e x i t ( E X I T _ F A I L U R E ) ; 	  
 	 }  
 	  
 	 c h a r *   f i l e n a m e   =   s t r t o k ( a r g v [ 1 ] ,   " . " ) ;  
 	 F I L E *   m i d i   =   f o p e n ( a r g v [ 1 ] ,   " r b " ) ;  
 	 i f   ( ! m i d i )  
 	 {  
 	 	 p r i n t f ( " C o u l d   n o t   o p e n   M I D I   F i l e . \ n " ) ;  
 	 	 e x i t ( E X I T _ F A I L U R E ) ;  
 	 }  
 	 c h a r   h e a d e r [ 5 ]   =   " " ;  
 	 f r e a d ( h e a d e r ,   1 ,   4 ,   m i d i ) ;  
 	 a s s e r t ( s t r c m p ( h e a d e r ,   " M T h d " )   = =   0 ) ;  
 	 f s e e k ( m i d i ,   3 ,   S E E K _ C U R ) ;  
 	 c h a r   l e n g t h ;  
 	 f r e a d ( & l e n g t h ,   1 ,   1 ,   m i d i ) ;  
 	 a s s e r t ( ( i n t ) l e n g t h   = =   6 ) ;  
 	 c h a r   m o d e ;  
 	 f s e e k ( m i d i ,   1 ,   S E E K _ C U R ) ;  
 	 f r e a d ( & m o d e ,   1 ,   1 ,   m i d i ) ;  
 	 u n s i g n e d   c h a r   n t r k [ 2 ] ;  
 	 f r e a d ( n t r k ,   1 ,   2 ,   m i d i ) ;  
 	 i n t   n u m t r a c k   =   n t r k [ 0 ]   < <   8   |   n t r k [ 1 ] ;  
 	 u n s i g n e d   c h a r   d i v [ 2 ] ;  
 	 f r e a d ( d i v ,   1 ,   2 ,   m i d i ) ;  
 	 u n s i g n e d   i n t   d i v i s i o n s   =   ( 0 x F F   &   d i v [ 0 ] )   < <   8   |   0 x F F   &   d i v [ 1 ] ;  
 	 p r i n t f ( " n t r k :   % u ,   d i v i s i o n s :   % u \ n " ,   n u m t r a c k ,   d i v i s i o n s ) ;  
 	 i f   ( m o d e   = =   0 b 0 0 0 0 )    
 	 {  
 	 	 p r i n t f ( " T h i s   M I D I   f i l e   i s   f o r m a t   0 . \ n " ) ;  
 	 	 P a r s e M o d e Z e r o ( m i d i ,   d i v i s i o n s ,   f i l e n a m e ) ;  
 	 }   / /   e n t e r   s i n g l e   t r a c k   m o d e  
 	 e l s e   i f   ( m o d e   = =   0 b 0 0 0 1 )  
 	 {  
 	 	 p r i n t f ( " T h i s   M I D I   f i l e   i s   f o r m a t   1 . \ n " ) ;  
 	 }   / /   e n t e r   s i m u l t a n e o u s   t r a c k   m o d e  
 	 e l s e   i f   ( m o d e   = =   0 b 0 0 1 0 )  
 	 {  
 	 	 p r i n t f ( " T h i s   M I D I   f i l e   u s e s   s e q u e n t i a l   t r a c k   m o d e ,   w h i c h   i s   n o t   y e t   s u p p o r t e d . \ n " ) ;  
 	 }   / /   s e q u e n t i a l   t r a c k   m o d e  
 	 f c l o s e ( m i d i ) ;  
 	 r e t u r n   0 ;  
 }  
  
 / /   R u n   p r o g r a m :   C t r l   +   F 5   o r   D e b u g   >   S t a r t   W i t h o u t   D e b u g g i n g   m e n u  
 / /   D e b u g   p r o g r a m :   F 5   o r   D e b u g   >   S t a r t   D e b u g g i n g   m e n u  
  
 / /   T i p s   f o r   G e t t i n g   S t a r t e d :    
 / /       1 .   U s e   t h e   S o l u t i o n   E x p l o r e r   w i n d o w   t o   a d d / m a n a g e   f i l e s  
 / /       2 .   U s e   t h e   T e a m   E x p l o r e r   w i n d o w   t o   c o n n e c t   t o   s o u r c e   c o n t r o l  
 / /       3 .   U s e   t h e   O u t p u t   w i n d o w   t o   s e e   b u i l d   o u t p u t   a n d   o t h e r   m e s s a g e s  
 / /       4 .   U s e   t h e   E r r o r   L i s t   w i n d o w   t o   v i e w   e r r o r s  
 / /       5 .   G o   t o   P r o j e c t   >   A d d   N e w   I t e m   t o   c r e a t e   n e w   c o d e   f i l e s ,   o r   P r o j e c t   >   A d d   E x i s t i n g   I t e m   t o   a d d   e x i s t i n g   c o d e   f i l e s   t o   t h e   p r o j e c t  
 / /       6 .   I n   t h e   f u t u r e ,   t o   o p e n   t h i s   p r o j e c t   a g a i n ,   g o   t o   F i l e   >   O p e n   >   P r o j e c t   a n d   s e l e c t   t h e   . s l n   f i l e  
 

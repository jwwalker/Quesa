FasdUAS 1.101.10   ��   ��    k             i         I      �������� (0 getmydirectorypath GetMyDirectoryPath��  ��    k     � 	 	  
  
 r     	    n         1    ��
�� 
psxp  l     ����  I    �� ��
�� .earsffdralis        afdr   f     ��  ��  ��    o      ���� 0 mypath myPath      r   
     n   
     1    ��
�� 
leng  o   
 ���� 0 mypath myPath  o      ���� 0 pathlen pathLen      l   ��������  ��  ��        l   ��  ��    "  Trim final slash if present     �   8   T r i m   f i n a l   s l a s h   i f   p r e s e n t       Z    2 ! "���� ! =    # $ # l    %���� % n     & ' & 4   �� (
�� 
cha  ( m    ������ ' o    ���� 0 mypath myPath��  ��   $ m     ) ) � * *  / " k    . + +  , - , r    ( . / . n    & 0 1 0 7   &�� 2 3
�� 
ctxt 2 m     ����  3 l  ! % 4���� 4 \   ! % 5 6 5 o   " #���� 0 pathlen pathLen 6 m   # $���� ��  ��   1 o    ���� 0 mypath myPath / o      ���� 0 mypath myPath -  7�� 7 r   ) . 8 9 8 l  ) , :���� : \   ) , ; < ; o   ) *���� 0 pathlen pathLen < m   * +���� ��  ��   9 o      ���� 0 pathlen pathLen��  ��  ��      = > = l  3 3��������  ��  ��   >  ? @ ? l  3 3�� A B��   A $  Trim off the application name    B � C C <   T r i m   o f f   t h e   a p p l i c a t i o n   n a m e @  D E D r   3 8 F G F n  3 6 H I H 1   4 6��
�� 
txdl I 1   3 4��
�� 
ascr G o      ���� 0 olddelim oldDelim E  J K J Q   9 ~ L M N L k   < b O O  P Q P r   < C R S R J   < ? T T  U�� U m   < = V V � W W  /��   S n      X Y X 1   @ B��
�� 
txdl Y 1   ? @��
�� 
ascr Q  Z [ Z r   D J \ ] \ n   D H ^ _ ^ 4  E H�� `
�� 
citm ` m   F G������ _ o   D E���� 0 mypath myPath ] o      ���� 0 appname appName [  a b a r   K \ c d c n   K Z e f e 7  L Z�� g h
�� 
ctxt g m   P R����  h l  S Y i���� i \   S Y j k j o   T U���� 0 pathlen pathLen k l  U X l���� l n   U X m n m 1   V X��
�� 
leng n o   U V���� 0 appname appName��  ��  ��  ��   f o   K L���� 0 mypath myPath d o      ���� 0 mypath myPath b  o�� o r   ] b p q p o   ] ^���� 0 olddelim oldDelim q n      r s r 1   _ a��
�� 
txdl s 1   ^ _��
�� 
ascr��   M R      �� t u
�� .ascrerr ****      � **** t o      ���� 0 m   u �� v w
�� 
errn v o      ���� 0 n   w �� x y
�� 
erob x o      ���� 0 f   y �� z {
�� 
errt z o      ���� 0 t   { �� |��
�� 
ptlr | o      ���� 0 p  ��   N k   j ~ } }  ~  ~ l  j j�� � ���   � : 4also reset text item delimiters in case of an error:    � � � � h a l s o   r e s e t   t e x t   i t e m   d e l i m i t e r s   i n   c a s e   o f   a n   e r r o r :   � � � r   j o � � � o   j k���� 0 olddelim oldDelim � n      � � � 1   l n��
�� 
txdl � 1   k l��
�� 
ascr �  � � � l  p p�� � ���   �  and resignal the error:    � � � � . a n d   r e s i g n a l   t h e   e r r o r : �  ��� � R   p ~�� � �
�� .ascrerr ****      � **** � o   | }���� 0 m   � �� � �
�� 
errn � o   r s���� 0 n   � �� � �
�� 
erob � o   t u���� 0 f   � �� � �
�� 
errt � o   v w���� 0 t   � �� ���
�� 
ptlr � o   x y���� 0 p  ��  ��   K  � � � l   ��������  ��  ��   �  ��� � L    � � � o    ����� 0 mypath myPath��     � � � l     ��������  ��  ��   �  � � � i     � � � I      �� ����� "0 getquesadirpath GetQuesaDirPath �  ��� � o      ���� 0 headerdocpath headerDocPath��  ��   � k     a � �  � � � r      � � � n     � � � 1    ��
�� 
txdl � 1     ��
�� 
ascr � o      ���� 0 olddelim oldDelim �  � � � r    	 � � � m     � � � � �   � o      ���� 0 
resultpath 
resultPath �  � � � Q   
 ^ � � � � k    D � �  � � � r     � � � J     � �  ��� � m     � � � � �  /��   � n      � � � 1    ��
�� 
txdl � 1    ��
�� 
ascr �  � � � r     � � � n     � � � 2   ��
�� 
citm � o    ���� 0 headerdocpath headerDocPath � o      ���� 	0 parts   �  � � � r    " � � � I    �� ���
�� .corecnte****       **** � o    ���� 	0 parts  ��   � o      ���� 0 numparts numParts �  � � � l  # #�� � ���   � 9 3 trim off "Development", "Scripts", "HeaderDoc", ""    � � � � f   t r i m   o f f   " D e v e l o p m e n t " ,   " S c r i p t s " ,   " H e a d e r D o c " ,   " " �  � � � Z   # > � ����� � ?   # & � � � o   # $���� 0 numparts numParts � m   $ %����  � r   ) : � � � c   ) 8 � � � l  ) 6 ����� � n   ) 6 � � � 7  * 6� � �
� 
cobj � m   . 0�~�~  � l  1 5 ��}�| � \   1 5 � � � o   2 3�{�{ 0 numparts numParts � m   3 4�z�z �}  �|   � o   ) *�y�y 	0 parts  ��  ��   � m   6 7�x
�x 
TEXT � o      �w�w 0 
resultpath 
resultPath��  ��   �  ��v � r   ? D � � � o   ? @�u�u 0 olddelim oldDelim � n      � � � 1   A C�t
�t 
txdl � 1   @ A�s
�s 
ascr�v   � R      �r � �
�r .ascrerr ****      � **** � o      �q�q 0 m   � �p � �
�p 
errn � o      �o�o 0 n   � �n � �
�n 
erob � o      �m�m 0 f   � �l � �
�l 
errt � o      �k�k 0 t   � �j ��i
�j 
ptlr � o      �h�h 0 p  �i   � k   L ^ � �  � � � l  L L�g � ��g   � : 4also reset text item delimiters in case of an error:    � � � � h a l s o   r e s e t   t e x t   i t e m   d e l i m i t e r s   i n   c a s e   o f   a n   e r r o r : �  � � � r   L Q � � � o   L M�f�f 0 olddelim oldDelim � n      � � � 1   N P�e
�e 
txdl � 1   M N�d
�d 
ascr �  � � � l  R R�c � �c   �  and resignal the error:     � . a n d   r e s i g n a l   t h e   e r r o r : � �b R   R ^�a
�a .ascrerr ****      � **** o   \ ]�`�` 0 m   �_
�_ 
errn o   T U�^�^ 0 n   �]
�] 
erob o   V W�\�\ 0 f   �[	

�[ 
errt	 o   X Y�Z�Z 0 t  
 �Y�X
�Y 
ptlr o   Z [�W�W 0 p  �X  �b   � �V L   _ a o   _ `�U�U 0 
resultpath 
resultPath�V   �  l     �T�S�R�T  �S  �R    i     I     �Q�P�O
�Q .aevtoappnull  �   � ****�P  �O   k     �  r      I     �N�M�L�N (0 getmydirectorypath GetMyDirectoryPath�M  �L   o      �K�K 0 mypath myPath  l   �J�I�H�J  �I  �H    l   �G�G   I Cdisplay dialog "My path: " & myPath buttons {"OK"} default button 1    � � d i s p l a y   d i a l o g   " M y   p a t h :   "   &   m y P a t h   b u t t o n s   { " O K " }   d e f a u l t   b u t t o n   1  !  l   �F�E�D�F  �E  �D  ! "�C" Z    �#$�B%# D    &'& o    	�A�A 0 mypath myPath' m   	 
(( �)) J / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c /$ k    y** +,+ r    -.- I    �@/�?�@ "0 getquesadirpath GetQuesaDirPath/ 0�>0 o    �=�= 0 mypath myPath�>  �?  . o      �<�< 0 	quesapath 	quesaPath, 121 r    343 n    565 1    �;
�; 
strq6 l   7�:�97 b    898 o    �8�8 0 	quesapath 	quesaPath9 m    :: �;; > / D o c u m e n t a t i o n / d e v e l o p e r / a p i r e f�:  �9  4 o      �7�7 0 
outputpath 
outputPath2 <=< r    &>?> n    $@A@ 1   " $�6
�6 
strqA l   "B�5�4B b    "CDC o     �3�3 0 	quesapath 	quesaPathD m     !EE �FF & / S D K / I n c l u d e s / Q u e s a�5  �4  ? o      �2�2 0 	inputpath 	inputPath= GHG r   ' .IJI n   ' ,KLK 1   * ,�1
�1 
strqL l  ' *M�0�/M b   ' *NON o   ' (�.�. 0 mypath myPathO m   ( )PP �QQ " h e a d e r D o c 2 H T M L . p l�0  �/  J o      �-�- 0 hdscriptpath hdScriptPathH RSR r   / 6TUT n   / 4VWV 1   2 4�,
�, 
strqW l  / 2X�+�*X b   / 2YZY o   / 0�)�) 0 mypath myPathZ m   0 1[[ �\\ $ g a t h e r H e a d e r D o c . p l�+  �*  U o      �(�( $0 gatherscriptpath gatherScriptPathS ]^] r   7 H_`_ b   7 Daba b   7 Bcdc b   7 >efe b   7 <ghg b   7 :iji m   7 8kk �ll 
 p e r l  j o   8 9�'�' 0 hdscriptpath hdScriptPathh m   : ;mm �nn    - o  f o   < =�&�& 0 
outputpath 
outputPathd m   > Aoo �pp   b o   B C�%�% 0 	inputpath 	inputPath` o      �$�$ 0 	hdcommand 	hdCommand^ qrq r   I Xsts b   I Tuvu b   I Rwxw b   I Nyzy m   I L{{ �|| 
 p e r l  z o   L M�#�# $0 gatherscriptpath gatherScriptPathx m   N Q}} �~~   v o   R S�"�" 0 
outputpath 
outputPatht o      �!�! 0 gathercommand gatherCommandr �  O   Y y��� k   _ x�� ��� I  _ d���
� .miscactvnull��� ��� null�  �  � ��� I  e x���
� .coredoscnull        TEXT� b   e t��� b   e p��� b   e l��� o   e h�� 0 	hdcommand 	hdCommand� m   h k�� ���  ;� o   l o�� 0 gathercommand gatherCommand� m   p s�� ��� . ; e c h o   ' . . . R e a l l y   d o n e . '�  �  � m   Y \���                                                                                      @  alis    \  Leopard                    ���-H+     �Terminal.app                                                     M�á��        ����  	                	Utilities     ��R�      â"{       �     +Leopard:Applications:Utilities:Terminal.app     T e r m i n a l . a p p    L e o p a r d  #Applications/Utilities/Terminal.app   / ��  �   �B  % I  | ����
� .sysodlogaskr        TEXT� b   | ���� b   | ���� b   | ���� b   | ���� m   | �� ��� � I   n e e d   t o   b e   i n   t h e   q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c /   f o l d e r .� l 	  ����� o    ��
� 
ret �  �  � o   � ��
� 
ret � m   � ��� ��� 2 I   a m   i n   t h e   w r o n g   p l a c e :  � o   � ��� 0 mypath myPath� ���
� 
btns� J   � ��� ��� m   � ��� ���  O K�  � ���
� 
dflt� m   � ��� �  �C   ��� l     ��
�	�  �
  �	  �       ���������������  � ������� ����������� (0 getmydirectorypath GetMyDirectoryPath� "0 getquesadirpath GetQuesaDirPath
� .aevtoappnull  �   � ****� 0 mypath myPath� 0 	quesapath 	quesaPath� 0 
outputpath 
outputPath�  0 	inputpath 	inputPath�� 0 hdscriptpath hdScriptPath�� $0 gatherscriptpath gatherScriptPath�� 0 	hdcommand 	hdCommand�� 0 gathercommand gatherCommand��  � �� ���������� (0 getmydirectorypath GetMyDirectoryPath��  ��  � 	�������������������� 0 mypath myPath�� 0 pathlen pathLen�� 0 olddelim oldDelim�� 0 appname appName�� 0 m  �� 0 n  �� 0 f  �� 0 t  �� 0 p  � �������� )������ V���������������
�� .earsffdralis        afdr
�� 
psxp
�� 
leng
�� 
cha 
�� 
ctxt
�� 
ascr
�� 
txdl
�� 
citm�� 0 m  � �����
�� 
errn�� 0 n  � �����
�� 
erob�� 0 f  � �����
�� 
errt�� 0 t  � ������
�� 
ptlr�� 0 p  ��  
�� 
errn
�� 
erob
�� 
errt
�� 
ptlr�� �� �)j  �,E�O��,E�O��i/�  �[�\[Zk\Z�k2E�O�kE�Y hO��,E�O +�kv��,FO��i/E�O�[�\[Zk\Z���,2E�O���,FW X 
 ���,FO)�����a �O�� �� ����������� "0 getquesadirpath GetQuesaDirPath�� ����� �  ���� 0 headerdocpath headerDocPath��  � 
���������������������� 0 headerdocpath headerDocPath�� 0 olddelim oldDelim�� 0 
resultpath 
resultPath�� 	0 parts  �� 0 numparts numParts�� 0 m  �� 0 n  �� 0 f  �� 0 t  �� 0 p  � ���� � ������������������������
�� 
ascr
�� 
txdl
�� 
citm
�� .corecnte****       ****�� 
�� 
cobj
�� 
TEXT�� 0 m  � �����
�� 
errn�� 0 n  � �����
�� 
erob�� 0 f  � �����
�� 
errt�� 0 t  � ������
�� 
ptlr�� 0 p  ��  
�� 
errn
�� 
erob
�� 
errt
�� 
ptlr�� �� b��,E�O�E�O <�kv��,FO��-E�O�j E�O�� �[�\[Zk\Z��2�&E�Y hO���,FW X 	 
���,FO)������O�� ����������
�� .aevtoappnull  �   � ****��  ��  �  � "����(����:����E��P��[��kmo��{}������������������������ (0 getmydirectorypath GetMyDirectoryPath�� 0 mypath myPath�� "0 getquesadirpath GetQuesaDirPath�� 0 	quesapath 	quesaPath
�� 
strq�� 0 
outputpath 
outputPath�� 0 	inputpath 	inputPath�� 0 hdscriptpath hdScriptPath�� $0 gatherscriptpath gatherScriptPath�� 0 	hdcommand 	hdCommand�� 0 gathercommand gatherCommand
�� .miscactvnull��� ��� null
�� .coredoscnull        TEXT
�� 
ret 
�� 
btns
�� 
dflt�� 
�� .sysodlogaskr        TEXT�� �*j+  E�O�� p*�k+ E�O��%�,E�O��%�,E�O��%�,E�O��%�,E�O��%�%�%a %�%E` Oa �%a %�%E` Oa  *j O_ a %_ %a %j UY %a _ %_ %a %�%a a kva ka   !� ��� z / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c /� ��� < / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a� ��� ~ ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D o c u m e n t a t i o n / d e v e l o p e r / a p i r e f '� ��� f ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / S D K / I n c l u d e s / Q u e s a '� ��� � ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c / h e a d e r D o c 2 H T M L . p l '� ��� � ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c / g a t h e r H e a d e r D o c . p l '� ���� p e r l   ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c / h e a d e r D o c 2 H T M L . p l '   - o   ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D o c u m e n t a t i o n / d e v e l o p e r / a p i r e f '   ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / S D K / I n c l u d e s / Q u e s a '� ���, p e r l   ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D e v e l o p m e n t / S c r i p t s / H e a d e r D o c / g a t h e r H e a d e r D o c . p l '   ' / V o l u m e s / W o r k / c l e a n q u e s a / q u e s a / D o c u m e n t a t i o n / d e v e l o p e r / a p i r e f '�  ascr  ��ޭ
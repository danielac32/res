

struct rt_hw_stack_frame
{
    uint32_t epc;        /* epc - epc    - program counter                     */
    uint32_t ra;         /* x1  - ra     - return address for jumps            */
    uint32_t a0;         /* x10 - a0     - return value or function argument 0 */
    
    uint32_t gp;         /* x3  - gp     - global pointer                      */
    uint32_t tp;         /* x4  - tp     - thread pointer                      */
    uint32_t t0;         /* x5  - t0     - temporary register 0                */
    uint32_t t1;         /* x6  - t1     - temporary register 1                */
    uint32_t t2;         /* x7  - t2     - temporary register 2                */
    uint32_t s0_fp;      /* x8  - s0/fp  - saved register 0 or frame pointer   */
    uint32_t s1;         /* x9  - s1     - saved register 1                    */
    uint32_t a1;         /* x11 - a1     - return value or function argument 1 */
    uint32_t a2;         /* x12 - a2     - function argument 2                 */
    uint32_t a3;         /* x13 - a3     - function argument 3                 */
    uint32_t a4;         /* x14 - a4     - function argument 4                 */
    uint32_t a5;         /* x15 - a5     - function argument 5                 */
    uint32_t a6;         /* x16 - a6     - function argument 6                 */
    uint32_t a7;         /* x17 - s7     - function argument 7                 */
    uint32_t s2;         /* x18 - s2     - saved register 2                    */
    uint32_t s3;         /* x19 - s3     - saved register 3                    */
    uint32_t s4;         /* x20 - s4     - saved register 4                    */
    uint32_t s5;         /* x21 - s5     - saved register 5                    */
    uint32_t s6;         /* x22 - s6     - saved register 6                    */
    uint32_t s7;         /* x23 - s7     - saved register 7                    */
    uint32_t s8;         /* x24 - s8     - saved register 8                    */
    uint32_t s9;         /* x25 - s9     - saved register 9                    */
    uint32_t s10;        /* x26 - s10    - saved register 10                   */
    uint32_t s11;        /* x27 - s11    - saved register 11                   */
    uint32_t t3;         /* x28 - t3     - temporary register 3                */
    uint32_t t4;         /* x29 - t4     - temporary register 4                */
    uint32_t t5;         /* x30 - t5     - temporary register 5                */
    uint32_t t6;         /* x31 - t6     - temporary register 6                */
};
module pso_funcion_objetivo
   use iso_c_binding

contains
   subroutine FuncionObjetivoF(VDP, CDV, RDO) bind(C, name="FuncionObjetivoF")
      INTEGER(C_INT), INTENT(IN), VALUE :: CDV
      !INTEGER(C_INT), INTENT(IN), VALUE :: CDP
      REAL(C_FLOAT), INTENT(INOUT) :: VDP(CDV)
      !REAL(C_FLOAT), INTENT(INOUT) :: PDO(CDP)
      REAL(C_FLOAT), INTENT(INOUT) :: RDO(1)
      INTEGER :: k
      !REAL :: H(5)
      ! Result variable initialization
      RDO(1) = 0
      ! H(1) = 1
      ! H(2) = 2
      ! H(3) = 3
      ! H(4) = 4
      ! H(5) = 5
      !WRITE (*, *) VDP(:)
      !WRITE (*, *) SUM(ABS(VDP(:)))
      !WRITE (*, *) PRODUCT(VDP(:))
      !WRITE (*, *) SUM(ABS(VDP(:)))
      !PRINT *, -(SUM(ABS(VDP(:))) + PRODUCT(VDP(:)))

  !!!!! Mathematical Functions for PSO Evaluation !!!!!
    !!! F2
      !RDO(1) = (SUM(ABS(VDP)) + PRODUCT(VDP))
    !!! F3
      !do k = 1, CDV, 1
      !   RDO = SUM(VDP(1:k)) + RDO
      !end do
    !!! F4
      RDO(1) = maxval(VDP)
    !!! F5
      !do k = 1, CDV - 1, 1
      !   RDO = RDO + ((100*((VDP(k + 1) - VDP(k)**2)**2)) + ((VDP(k) - 1)**2))
      !end do
    !!! F6
      !RDO(1) = SUM((floor(VDP + 1.0/2)))
    !!! F7
      !
    !!! F8
      !RDO(1) = SUM(-VDP*SIN(SQRT(ABS(VDP))))
   end subroutine
end module

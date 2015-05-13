/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */
/*
  Unit tests for Matrix struct

  Test data was randomly generated and the expected results
  calculated using GNU Octave.
 */

#include "math/func.h"
#include "math/matrix.h"

#include <gtest/gtest.h>


const float TEST_TOLERANCE = 1e-6f;


TEST(MatrixTest, TransposeTest)
{
    const Math::Matrix mat(
        {
            { -0.07011674491203920f,  1.26145596067429810f,  2.09476603598066902f,  0.35560176915570696f },
            { -1.34075615966224704f,  1.17988499016709314f,  0.00601713429241016f, -0.75213676977972566f },
            {  0.59186722295223981f,  0.88089224074765293f,  0.70994467464257294f,  0.36730385425340212f },
            { -0.95649396555068111f,  0.75912182022565566f,  1.34883305778387186f, -1.34957997578168754f }
        }
    );

    const Math::Matrix expectedTranspose(
        {
            { -0.07011674491203920f, -1.34075615966224704f,  0.59186722295223981f, -0.95649396555068111f },
            {  1.26145596067429810f,  1.17988499016709314f,  0.88089224074765293f,  0.75912182022565566f },
            {  2.09476603598066902f,  0.00601713429241016f,  0.70994467464257294f,  1.34883305778387186f },
            {  0.35560176915570696f, -0.75213676977972566f,  0.36730385425340212f, -1.34957997578168754f }
        }
    );

    Math::Matrix transpose = Math::Transpose(mat);

    EXPECT_TRUE(Math::MatricesEqual(transpose, expectedTranspose, TEST_TOLERANCE));
}

TEST(MatrixTest, CofactorTest)
{
    const Math::Matrix mat1(
        {
            {  0.610630320796245f,  1.059932357918312f, -1.581674311378210f,  1.782214448453331f },
            {  0.191028848211526f, -0.813898708757524f,  1.516114203870644f,  0.395202639476002f },
            {  0.335142750345279f, -0.346586619596529f,  0.545382042472336f, -0.879268918923072f },
            {  1.417588151657198f,  1.450841789070141f,  0.219080104196171f,  0.378724047481655f }
        }
    );

    const Math::Matrix expectedCofactors1(
        {
            { -2.402679369186782f,  2.282452509293019f,  1.722732204057644f, -0.746939701104385f },
            { -0.687677756877654f,  1.168949180331164f, -0.985354966837796f, -1.334071111592705f },
            { -5.115621958424845f,  4.229724770159009f,  2.529000630782808f,  1.481632618355891f },
            {  0.147480897398694f, -2.140677680337111f, -1.207189492265546f,  0.151236920408051f }
        }
    );

    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            float ret = mat1.Cofactor(r, c);
            float exp = expectedCofactors1.m[4*c+r];
            EXPECT_TRUE(Math::IsEqual(ret, exp, TEST_TOLERANCE));
        }
    }

    const Math::Matrix mat2(
        {
            {  0.9845099464982393f, -0.9091233416532389f, -0.6272243714245945f,  0.4645001858944354f },
            { -0.1333308471483736f,  0.9128181433725897f, -1.0937461393836190f,  0.3180936795928376f },
            { -0.0654324396846289f,  0.1014641705415945f,  1.5107709042683430f, -0.0240560430414690f },
            {  0.0179638644093347f, -1.0695585982782767f, -0.1741250853101032f,  1.0803106709464336f }
        }
    );

    const Math::Matrix expectedCofactors2(
        {
            {  2.0861102207614466f,  0.2989010779528912f,  0.0746276150537432f,  0.2732659822656097f },
            {  0.6850002886584565f,  1.5513169659641379f, -0.0503743176545917f,  1.5163672441575642f },
            {  1.2385556680997216f,  1.1827709562505695f,  1.2282813085138962f,  1.3483789679871401f },
            { -1.0710790241539783f, -0.5589604503588883f,  0.0100959837872308f,  1.1897872684455839f }
        }
    );


    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            float ret = mat2.Cofactor(r, c);
            float exp = expectedCofactors2.m[4*c+r];
            EXPECT_TRUE(Math::IsEqual(ret, exp, TEST_TOLERANCE));
        }
    }
}

TEST(MatrixTest, DetTest)
{
    const Math::Matrix mat1(
        {
            { -0.95880162984708284f,  0.24004047608997131f, -0.78172309932665407f, -0.11604124457222834f },
            { -0.36230592086261376f, -0.75778166876017261f,  0.33041059404631740f, -1.06001391941094836f },
            {  0.00260215210936187f,  1.27485610196385113f, -0.26149859846418033f, -0.59669701186364876f },
            {  0.36899429848485432f,  3.01720896813933104f,  2.10311476609438719f, -1.68627076626448269f }
        }
    );

    const float expectedDet1 = 4.07415413729671f;

    float ret1 = mat1.Det();
    EXPECT_TRUE(Math::IsEqual(ret1, expectedDet1, TEST_TOLERANCE));

    const Math::Matrix mat2(
        {
            { -1.0860073221346871f,  0.9150354098189495f, -0.2723201933559999f,  0.2922832160271507f },
            { -1.0248331304801788f, -2.5081237461125205f, -1.0277123574586633f, -0.2254690663329798f },
            { -1.4227635282899367f, -0.0403846809122684f,  0.9216148477171653f,  1.2517067488015878f },
            { -0.1160254467152022f,  0.8270675274393656f,  1.0327218739781614f, -0.3674886870220400f }
        }
    );

    const float expectedDet2 = -6.35122307880942f;

    float ret2 = mat2.Det();
    EXPECT_TRUE(Math::IsEqual(ret2, expectedDet2, TEST_TOLERANCE));
}

TEST(MatrixTest, InverseTest)
{
    const Math::Matrix mat1(
        {
            { -2.2829352811514658f, -0.9103222363187888f,  0.2792976509411680f, -0.7984393573193174f },
            {  2.4823665798689589f, -0.0599056759070980f,  0.3832364352926366f, -1.6404257204372739f },
            { -0.3841952272526398f, -0.8377700696457873f, -0.3416328338427138f,  1.1746577275723329f },
            {  0.1746031241954947f, -0.4952532117949962f,  0.2155084379835037f, -1.6586460437329220f }
        }
    );

    const Math::Matrix expectedInverse1(
        {
            { -0.119472603171041f,  0.331675963276297f,  0.187516809009720f, -0.137720814290806f },
            { -0.387591686166085f, -0.487284946727583f, -0.798527541290274f,  0.102991635972060f },
            {  2.601905603425902f,  2.606899016264679f, -0.528006148839176f, -4.204703326522837f },
            {  0.441220327151392f,  0.519128136207318f,  0.189567009205522f, -1.194469716136194f }
        }
    );

    Math::Matrix inverse1 = mat1.Inverse();

    EXPECT_TRUE(Math::MatricesEqual(inverse1, expectedInverse1, TEST_TOLERANCE));

    const Math::Matrix mat2(
        {
            { -0.05464332404298505f, -0.64357755258235749f, -0.13017671677619302f, -0.56742332785888006f },
            {  0.29048383600458222f, -0.91517047043724875f,  0.84517524415561684f,  0.51628195547960565f },
            {  0.00946488004480186f, -0.89077382212689293f,  0.73565573766341397f, -0.15932513521840930f },
            { -1.01244718912499132f, -0.27840911963972276f, -0.39189681211309862f,  1.18315064340192055f }
        }
    );

    const Math::Matrix expectedInverse2(
        {
            {  0.771302711132012f,  1.587542278361995f, -2.003075114445104f, -0.592574156227379f },
            { -1.208929259769431f, -0.786598967848473f,  0.607335305808052f, -0.154759693303324f },
            { -1.500037668208218f, -0.774300278997914f,  1.917800427261255f, -0.123268572651291f },
            { -0.121314770937944f,  0.916925149209746f, -0.935924950785014f,  0.260875394250671f }
        }
    );

    Math::Matrix inverse2 = mat2.Inverse();

    EXPECT_TRUE(Math::MatricesEqual(inverse2, expectedInverse2, TEST_TOLERANCE));
}

TEST(MatrixTest, MultiplyTest)
{
    const Math::Matrix mat1A(
        {
            {  0.6561727049162027f, -1.4180263627131411f, -0.8271026046117423f,  2.3919331748512578f },
            { -0.6035665535146352f,  0.0150827348790615f, -0.7090794192822540f,  0.9057604704594814f },
            { -0.9871045001223655f, -0.4980646811455065f,  0.3806177002298990f,  0.1520583649240934f },
            { -0.2721911170792712f,  0.7627928194552067f, -0.1504091336784158f,  0.9747545351840121f }
        }
    );

    const Math::Matrix mat1B(
        {
            { -0.2643735892448818f, -0.7542994492819621f,  0.6082322350568750f,  0.0581733424861419f },
            {  1.0293246070431237f,  0.1979285388251341f, -0.2932031385332818f,  0.8838407179018929f },
            {  0.3448687251553114f,  0.5031654871245456f,  0.7554693012922442f, -0.4845315903845708f },
            { -1.8662838497278593f, -0.7843850624747805f,  0.1389026096476257f, -1.3686415408300689f }
        }
    );

    const Math::Matrix expectedMultiply1(
        {
            { -6.382352236417988f, -3.067984733682130f,  0.522270304251466f, -4.088079444498280f },
            { -1.759853366848825f, -0.608994052024491f, -0.781406179437379f, -0.917870775786188f },
            { -0.404226802169062f,  0.718232546720114f, -0.145688356880835f, -0.890167707987175f },
            { -1.013918490922430f, -0.483971504099758f, -0.367442194643757f, -0.602858486133615f }
        }
    );

    Math::Matrix multiply1 = Math::MultiplyMatrices(mat1A, mat1B);
    EXPECT_TRUE(Math::MatricesEqual(multiply1, expectedMultiply1, TEST_TOLERANCE));

    const Math::Matrix mat2A(
        {
            {  0.8697203025776754f,  2.1259475710644935f,  1.7856691009707812f, -2.1563963348328126f },
            {  1.5888074489288735f, -0.0794849733953615f,  0.7307782768677457f,  0.7943129159612630f },
            {  0.2859761537233830f, -0.6231231890384962f, -0.0496743172880377f, -0.8137857518646087f },
            {  1.2670547229512983f, -0.5305171374831831f, -0.4987412674062375f, -1.1257327113869595f }
        }
    );

    const Math::Matrix mat2B(
        {
            {  1.1321105701165317f,  0.1759563504574463f, -2.0675778912000418f,  1.4840339814245538f },
            { -1.5117280888829916f, -0.0933013188828093f, -0.2079262944351640f,  0.9575727579539316f },
            {  0.3615378398970173f,  1.2465163589027248f,  1.1326150997082589f,  0.9921208694352303f },
            { -0.7357104529373861f, -0.4774022005969588f, -0.2118739096676499f,  1.1427567093270703f }
        }
    );

    const Math::Matrix expectedMultiply2(
        {
            {  0.00283516267056338f,  3.21001319965989307f,  0.23910503934370686f,  2.63380716363006107f },
            {  1.59868505822469742f,  0.81869715594617765f, -2.60905981088293570f,  3.91445839239110294f },
            {  1.84650099286297942f,  0.43504079532852930f, -0.34555619012424243f, -1.15152951542451487f },
            {  2.88434318563174585f,  0.18818239851585700f, -2.83579436909308980f, -0.40890672198610400f }
        }
    );

    Math::Matrix multiply2 = Math::MultiplyMatrices(mat2A, mat2B);
    EXPECT_TRUE(Math::MatricesEqual(multiply2, expectedMultiply2, TEST_TOLERANCE));
}

TEST(MatrixTest, MultiplyVectorTest)
{
    const Math::Matrix mat1(
        {
            {  0.188562846910008f, -0.015148651460679f,  0.394512304108827f,  0.906910631257135f },
            { -0.297506779519667f,  0.940119328178913f,  0.970957796752517f,  0.310559318965526f },
            { -0.819770525290873f, -2.316574438778879f,  0.155756069319732f, -0.855661405742964f },
            {  0.000000000000000f,  0.000000000000000f,  0.000000000000000f,  1.000000000000000f }
        }
    );

    const Math::Vector vec1(-0.824708565156661f, -1.598287748103842f, -0.422498044734181f);

    const Math::Vector expectedMultiply1(0.608932463260470f, -1.356893266403749f, 3.457156276255142f);

    Math::Vector multiply1 = Math::MatrixVectorMultiply(mat1, vec1, false);
    EXPECT_TRUE(Math::VectorsEqual(multiply1, expectedMultiply1, TEST_TOLERANCE));

    const Math::Matrix mat2(
        {
            { -0.63287117038834284f,  0.55148060401816856f, -0.02042395559467368f, -1.50367083897656850f },
            {  0.69629042156335297f,  0.12982747869796774f, -1.16250029235919405f,  1.19084447253756909f },
            {  0.44164132914357224f, -0.15169304045662041f, -0.00880583574621390f, -0.55817802940035310f },
            {  0.95680476533530789f, -1.51912346889253125f, -0.74209769406615944f, -0.20938988867903682f }
        }
    );

    const Math::Vector vec2(0.330987381051962f, 1.494375516393466f, 1.483422335561857f);

    const Math::Vector expectedMultiply2(0.2816820577317669f, 0.0334468811767428f, 0.1996974284970455f);

    Math::Vector multiply2 = Math::MatrixVectorMultiply(mat2, vec2, true);
    EXPECT_TRUE(Math::VectorsEqual(multiply2, expectedMultiply2, TEST_TOLERANCE));
}

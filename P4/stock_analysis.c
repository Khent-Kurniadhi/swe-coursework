/*
 * stock_analysis.c
 * Stock Performance Calculator
 * Calculates ROI, Dividend Yield, Graham Number, P/E Ratio, EPS & EPS Growth.
 *
 * Compile & run:
 *   gcc -o stock_analysis stock_analysis.c -lm
 *   ./stock_analysis
 *
 * Compile & run tests (requires CUnit):
 *   gcc -o stock_tests stock_analysis.c LibCUnit/Automated.c LibCUnit/CUError.c LibCUnit/TestDB.c LibCUnit/TestRun.c LibCUnit/Util.c LibCUnit/Basic.c LibCUnit/MyMem.c LibCUnit/Console.c -I./LibCUnit -lm -DRUN_TESTS
 *   ./stock_tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifdef RUN_TESTS
#include "CUnit.h"
#include "Basic.h"
#endif

/* ─────────────────────────────────────────────
   Data Structures
───────────────────────────────────────────── */

typedef struct {
    char   ticker[16];
    double buy_price;          /* price paid per share          */
    double current_price;      /* current market price          */
    int    shares;             /* number of shares held         */
    double annual_dividend;    /* annual dividend per share (0 if none) */
    double eps;                /* earnings per share            */
    double eps_previous;       /* EPS from prior period         */
    double book_value_per_share; /* book value per share        */
} StockData;

typedef struct {
    double roi_percent;        /* Return on Investment (%)      */
    double dividend_yield;     /* Dividend Yield (%)            */
    double pe_ratio;           /* Price-to-Earnings ratio       */
    double eps_growth_percent; /* EPS growth rate (%)           */
    double graham_number;      /* Graham intrinsic value        */
    double margin_of_safety;   /* (Graham - Price) / Graham (%) */
    double total_gain_loss;    /* Absolute gain/loss in currency*/
} StockResults;

/* ─────────────────────────────────────────────
   Calculation Functions
───────────────────────────────────────────── */

/*
 * calculate_roi
 * ROI (%) = ((Current Price - Buy Price) / Buy Price) * 100
 */
double calculate_roi(double buy_price, double current_price)
{
    if (buy_price <= 0.0) return 0.0;
    return ((current_price - buy_price) / buy_price) * 100.0;
}

/*
 * calculate_total_gain_loss
 * Total gain/loss = (Current Price - Buy Price) * Shares
 */
double calculate_total_gain_loss(double buy_price, double current_price, int shares)
{
    return (current_price - buy_price) * (double)shares;
}

/*
 * calculate_dividend_yield
 * Dividend Yield (%) = (Annual Dividend / Current Price) * 100
 */
double calculate_dividend_yield(double annual_dividend, double current_price)
{
    if (current_price <= 0.0 || annual_dividend <= 0.0) return 0.0;
    return (annual_dividend / current_price) * 100.0;
}

/*
 * calculate_pe_ratio
 * P/E = Current Price / EPS
 */
double calculate_pe_ratio(double current_price, double eps)
{
    if (eps == 0.0) return 0.0;
    return current_price / eps;
}

/*
 * calculate_eps_growth
 * EPS Growth (%) = ((EPS_current - EPS_previous) / |EPS_previous|) * 100
 */
double calculate_eps_growth(double eps_current, double eps_previous)
{
    if (eps_previous == 0.0) return 0.0;
    return ((eps_current - eps_previous) / fabs(eps_previous)) * 100.0;
}

/*
 * calculate_graham_number
 * Graham Number = sqrt(22.5 * EPS * Book Value Per Share)
 * Represents the maximum price a defensive investor should pay.
 * Returns 0 if inputs are invalid (negative/zero).
 */
double calculate_graham_number(double eps, double book_value_per_share)
{
    if (eps <= 0.0 || book_value_per_share <= 0.0) return 0.0;
    return sqrt(22.5 * eps * book_value_per_share);
}

/*
 * calculate_margin_of_safety
 * MoS (%) = ((Graham Number - Current Price) / Graham Number) * 100
 * Positive = stock is undervalued; Negative = stock is overvalued.
 */
double calculate_margin_of_safety(double graham_number, double current_price)
{
    if (graham_number <= 0.0) return 0.0;
    return ((graham_number - current_price) / graham_number) * 100.0;
}

/* ─────────────────────────────────────────────
   Aggregate: run all calculations
───────────────────────────────────────────── */

StockResults analyse_stock(const StockData *s)
{
    StockResults r;
    memset(&r, 0, sizeof(r));

    r.roi_percent        = calculate_roi(s->buy_price, s->current_price);
    r.total_gain_loss    = calculate_total_gain_loss(s->buy_price, s->current_price, s->shares);
    r.dividend_yield     = calculate_dividend_yield(s->annual_dividend, s->current_price);
    r.pe_ratio           = calculate_pe_ratio(s->current_price, s->eps);
    r.eps_growth_percent = calculate_eps_growth(s->eps, s->eps_previous);
    r.graham_number      = calculate_graham_number(s->eps, s->book_value_per_share);
    r.margin_of_safety   = calculate_margin_of_safety(r.graham_number, s->current_price);

    return r;
}

/* ─────────────────────────────────────────────
   Display / Report
───────────────────────────────────────────── */

void print_separator(void)
{
    printf("─────────────────────────────────────────────\n");
}

void print_results(const StockData *s, const StockResults *r)
{
    print_separator();
    printf("  STOCK ANALYSIS REPORT : %s\n", s->ticker);
    print_separator();

    printf("\n  [ Position ]\n");
    printf("  %-28s %10.2f\n", "Buy Price ($):",        s->buy_price);
    printf("  %-28s %10.2f\n", "Current Price ($):",    s->current_price);
    printf("  %-28s %10d\n",   "Shares Held:",          s->shares);
    printf("  %-28s %+10.2f\n","Total Gain / Loss ($):", r->total_gain_loss);
    printf("  %-28s %+9.2f%%\n","ROI:",                  r->roi_percent);

    printf("\n  [ Dividends ]\n");
    if (s->annual_dividend > 0.0) {
        printf("  %-28s %10.2f\n", "Annual Dividend / Share ($):", s->annual_dividend);
        printf("  %-28s %9.2f%%\n", "Dividend Yield:",              r->dividend_yield);
    } else {
        printf("  No dividend data provided.\n");
    }

    printf("\n  [ Valuation Metrics ]\n");
    printf("  %-28s %10.2f\n",  "EPS ($):",             s->eps);
    printf("  %-28s %10.2f\n",  "Previous EPS ($):",    s->eps_previous);
    if (s->eps_previous != 0.0)
        printf("  %-28s %+9.2f%%\n","EPS Growth:",       r->eps_growth_percent);

    if (s->eps == 0.0)
        printf("  P/E Ratio:         N/A (EPS is zero)\n");
    else if (r->pe_ratio < 0.0)
        printf("  %-28s %10.2f  ⚠ Net loss (negative EPS)\n", "P/E Ratio:", r->pe_ratio);
    else
        printf("  %-28s %10.2f\n", "P/E Ratio:", r->pe_ratio);

    printf("\n  [ Graham Intrinsic Value ]\n");
    printf("  %-28s %10.2f\n", "Book Value / Share ($):", s->book_value_per_share);
    if (r->graham_number > 0.0) {
        printf("  %-28s %10.2f\n",  "Graham Number ($):",   r->graham_number);
        printf("  %-28s %+9.2f%%\n","Margin of Safety:",    r->margin_of_safety);
        if (r->margin_of_safety >= 20.0)
            printf("  ★  Stock appears UNDERVALUED (MoS ≥ 20%%)\n");
        else if (r->margin_of_safety >= 0.0)
            printf("  ◑  Stock is near fair value\n");
        else
            printf("  ✗  Stock appears OVERVALUED vs Graham Number\n");
    } else {
        printf("  Graham Number:     N/A (EPS or Book Value <= 0)\n");
    }

    print_separator();
}

/* ─────────────────────────────────────────────
   User Input
───────────────────────────────────────────── */

int get_double(const char *prompt, double *out, double min_val, double max_val)
{
    double val;
    printf("  %s", prompt);
    // 1. Attempt to read the number
    int result = scanf("%lf", &val);

    // 2. THE FIX: Always clear the buffer until the end of the line
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // 3. Now check if the read was actually successful
    if (result == EOF) {
        printf("\n  ! Unexpected end of input. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    if (result != 1) {
        printf("  ! Invalid input. Please enter a numerical value.\n");
        return 0;
    }

    if (val < min_val || val > max_val) {
        printf("  ! Value must be between %.2f and %.2f. Try again.\n", min_val, max_val);
        return 0;
    }
    *out = val;
    return 1;
}

int get_int(const char *prompt, int *out, int min_val, int max_val)
{
    int val;
    printf("  %s", prompt);
    // 1. Attempt to read the number
    int result = scanf("%d", &val);

    // 2. THE FIX: Always clear the buffer until the end of the line
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    // 3. Now check if the read was actually successful
    if (result == EOF) {
        printf("\n  ! Unexpected end of input. Exiting.\n");
        exit(EXIT_FAILURE);
    }

    if (result != 1) {
        printf("  ! Invalid input. Please enter a numerical value.\n");
        return 0;
    }
    if (val < min_val || val > max_val) {
        printf("  ! Value must be between %d and %d Try again.\n", min_val, max_val);
        return 0;
    }
    *out = val;
    return 1;
}

void collect_input(StockData *s)
{
    print_separator();
    printf("  STOCK PERFORMANCE CALCULATOR\n");
    print_separator();
    printf("\n  Enter ticker symbol: ");
    fgets(s->ticker, sizeof(s->ticker), stdin);
    // strip trailing newline
    s->ticker[strcspn(s->ticker, "\n")] = '\0';

    while (!get_double("Buy price per share ($): ",     &s->buy_price,            0.01,100000.0));
    while (!get_double("Current price per share ($): ", &s->current_price,        0.01,100000.0));
    while (!get_int   ("Number of shares held: ",       &s->shares,               1,100000));
    while (!get_double("Annual dividend per share (0 if none): ", &s->annual_dividend, 0.0,100000.0));
    while (!get_double("EPS (Earnings Per Share, $): ", &s->eps,                  -100000.0,100000.0));
    while (!get_double("Previous period EPS ($, 0 if unknown): ", &s->eps_previous, -100000.0,100000.0));
    while (!get_double("Book value per share ($): ",    &s->book_value_per_share, 0.01,100000.0));
}

/* ─────────────────────────────────────────────
   CUnit Tests
───────────────────────────────────────────── */

#ifdef RUN_TESTS //only compile if RUN_TEST is defined

#define EPSILON 1e-6

void test_roi_positive(void)
{
    double result = calculate_roi(100.0, 150.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 50.0, EPSILON); // verify the result within a tiny margin or error
}

void test_roi_negative(void)
{
    double result = calculate_roi(200.0, 160.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, -20.0, EPSILON); // verify the result within a tiny margin or error
}

void test_roi_zero_buy_price(void)
{
    double result = calculate_roi(0.0, 150.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 0.0, EPSILON); // verify the result within a tiny margin or error
}

void test_total_gain_loss(void)
{
    double result = calculate_total_gain_loss(50.0, 75.0, 100); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 2500.0, EPSILON); // verify the result within a tiny margin or error
}

void test_total_gain_loss_negative(void)
{
    double result = calculate_total_gain_loss(100.0, 80.0, 10); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, -200.0, EPSILON); // verify the result within a tiny margin or error
}

void test_dividend_yield(void)
{
    double result = calculate_dividend_yield(2.0, 40.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 5.0, EPSILON); // verify the result within a tiny margin or error
}

void test_dividend_yield_no_dividend(void)
{
    double result = calculate_dividend_yield(0.0, 100.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 0.0, EPSILON); // verify the result within a tiny margin or error
}

void test_pe_ratio(void)
{
    double result = calculate_pe_ratio(150.0, 7.5); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 20.0, EPSILON); // verify the result within a tiny margin or error
}

void test_pe_ratio_zero_eps(void)
{
    // Zero EPS should return 0 (guard against division by zero)
    double result = calculate_pe_ratio(150.0, 0.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 0.0, EPSILON); // verify the result within a tiny margin or error
}

void test_pe_ratio_negative_eps(void)
{
    // Negative EPS = company at a loss, P/E should be negative
    double result = calculate_pe_ratio(100.0, -5.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, -20.0, EPSILON); // verify the result within a tiny margin or error
}

void test_eps_growth_positive(void)
{
    double result = calculate_eps_growth(6.0, 5.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 20.0, EPSILON); // verify the result within a tiny margin or error
}

void test_eps_growth_negative(void)
{
    double result = calculate_eps_growth(3.0, 4.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, -25.0, EPSILON); // verify the result within a tiny margin or error
}

void test_eps_growth_zero_previous(void)
{
    double result = calculate_eps_growth(5.0, 0.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 0.0, EPSILON); // verify the result within a tiny margin or error
}

void test_graham_number(void)
{
    /* sqrt(22.5 * 5.0 * 20.0) = sqrt(2250) ≈ 47.4342 */
    double result = calculate_graham_number(5.0, 20.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, sqrt(2250.0), EPSILON); // verify the result within a tiny margin or error
}

void test_graham_number_invalid(void)
{
    // Test Case 1: Ensure result is 0.0 when EPS is invalid (0.0)
    CU_ASSERT_DOUBLE_EQUAL(calculate_graham_number(0.0, -20.0), 0.0, EPSILON);
    // Test Case 2: Ensure result is 0.0 when Book Value is invalid (0.0)
    CU_ASSERT_DOUBLE_EQUAL(calculate_graham_number(5.0, 0.0),  0.0, EPSILON);
}

void test_margin_of_safety_undervalued(void)
{
    /* Graham = 50, Price = 35  =>  MoS = (50-35)/50 * 100 = 30% */
    double result = calculate_margin_of_safety(50.0, 35.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 30.0, EPSILON); // verify the result within a tiny margin or error
}

void test_margin_of_safety_overvalued(void)
{
    /* Graham = 40, Price = 60  =>  MoS = (40-60)/40 * 100 = -50% */
    double result = calculate_margin_of_safety(40.0, 60.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, -50.0, EPSILON); // verify the result within a tiny margin or error
}

void test_margin_of_safety_zero_graham(void)
{
    double result = calculate_margin_of_safety(0.0, 50.0); // calls the logic function
    CU_ASSERT_DOUBLE_EQUAL(result, 0.0, EPSILON); // verify the result within a tiny margin or error
}

// This function creates a fake stock to be tested which includes all test function
void test_analyse_stock_integration(void)
{
    StockData s;
    memset(&s, 0, sizeof(s));
    strncpy(s.ticker, "TST", sizeof(s.ticker) - 1);
    s.buy_price            = 100.0;
    s.current_price        = 130.0;
    s.shares               = 50;
    s.annual_dividend      = 2.60;
    s.eps                  = 6.50;
    s.eps_previous         = 5.00;
    s.book_value_per_share = 30.0;

    StockResults r = analyse_stock(&s);

    CU_ASSERT_DOUBLE_EQUAL(r.roi_percent,        30.0,  EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.total_gain_loss,   1500.0, EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.dividend_yield,     2.0,   EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.pe_ratio,          20.0,   EPSILON);
    CU_ASSERT_DOUBLE_EQUAL(r.eps_growth_percent, 30.0,  EPSILON);
}

/* ── Test Registry: 2 separate suites ── */
int main_tests(void)
{
    if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

    /* ── Suite 1: Returns & Income ─────────────────────────────────────
     * Covers: calculate_roi, calculate_total_gain_loss,
     *         calculate_dividend_yield
     * ────────────────────────────────────────────────────────────────── */
    CU_pSuite suite_returns = CU_add_suite("Suite 1 - Returns & Income", NULL, NULL);
    if (!suite_returns) { CU_cleanup_registry(); return CU_get_error(); }

    CU_add_test(suite_returns, "ROI: profit scenario",          test_roi_positive);
    CU_add_test(suite_returns, "ROI: loss scenario",            test_roi_negative);
    CU_add_test(suite_returns, "ROI: zero buy price guard",     test_roi_zero_buy_price);
    CU_add_test(suite_returns, "Gain/Loss: positive return",    test_total_gain_loss);
    CU_add_test(suite_returns, "Gain/Loss: negative return",    test_total_gain_loss_negative);
    CU_add_test(suite_returns, "Dividend Yield: normal",        test_dividend_yield);
    CU_add_test(suite_returns, "Dividend Yield: no dividend",   test_dividend_yield_no_dividend);

    /* ── Suite 2: Valuation Metrics ────────────────────────────────────
     * Covers: calculate_pe_ratio, calculate_eps_growth,
     *         calculate_graham_number, calculate_margin_of_safety,
     *         analyse_stock (integration)
     * ────────────────────────────────────────────────────────────────── */
    CU_pSuite suite_valuation = CU_add_suite("Suite 2 - Valuation Metrics", NULL, NULL);
    if (!suite_valuation) { CU_cleanup_registry(); return CU_get_error(); }

    CU_add_test(suite_valuation, "P/E Ratio: valid EPS",          test_pe_ratio);
    CU_add_test(suite_valuation, "P/E Ratio: zero EPS guard",     test_pe_ratio_zero_eps);
    CU_add_test(suite_valuation, "EPS Growth: improvement",       test_eps_growth_positive);
    CU_add_test(suite_valuation, "EPS Growth: decline",           test_eps_growth_negative);
    CU_add_test(suite_valuation, "EPS Growth: zero previous guard",test_eps_growth_zero_previous);
    CU_add_test(suite_valuation, "Graham Number: valid inputs",   test_graham_number);
    CU_add_test(suite_valuation, "Graham Number: invalid inputs", test_graham_number_invalid);
    CU_add_test(suite_valuation, "Margin of Safety: undervalued", test_margin_of_safety_undervalued);
    CU_add_test(suite_valuation, "Margin of Safety: overvalued",  test_margin_of_safety_overvalued);
    CU_add_test(suite_valuation, "Margin of Safety: zero graham", test_margin_of_safety_zero_graham);
    CU_add_test(suite_valuation, "Integration: full analysis",    test_analyse_stock_integration);
    CU_add_test(suite_valuation, "P/E Ratio: negative EPS (net loss)", test_pe_ratio_negative_eps);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    unsigned int failures = CU_get_number_of_failures();
    CU_cleanup_registry();
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

#endif /* RUN_TESTS */

/* ─────────────────────────────────────────────
   Entry Points
───────────────────────────────────────────── */

#ifdef RUN_TESTS
int main(void) { return main_tests(); }
#else
int main(void)
{
    StockData    stock;
    memset(&stock, 0, sizeof(stock));

    collect_input(&stock);

    StockResults results = analyse_stock(&stock);
    printf("\n");
    print_results(&stock, &results);

    return EXIT_SUCCESS;
}
#endif
